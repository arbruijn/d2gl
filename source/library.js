var ctx, img;
var width = 320, height = 200;
function mydraw() {
	if (!ctx) {
		var canvas = document.querySelector("canvas")
		canvas.width = width;
		canvas.height = height;
		ctx = canvas.getContext("2d");
		//data = new Uint8ClampedArray(Module.HEAP, pointer, width * height * 4);
		var pointer = Module._get_screen();
		//data = Module.HEAPU8.subarray(pointer, width * height * 4);
		var data = new Uint8ClampedArray(Module.HEAPU8.buffer, pointer, width * height * 4);
		img = new ImageData(data, width, height);
	}
	ctx.putImageData(img, 0, 0);
}

var playerModule;

function fixAudio() {
	console.log('fixAudio');
	if (!playerModule)
		return;
	if (!audioContext)
		nextBuf();
	else if (audioContext.state == 'suspended')
		audioContext.resume().then(() => {
			console.log('Playback resumed successfully');
			audioInit2();
			nextBuf();
		});
}

async function loadInto(url, buf, ofs) {
	var data = await (await fetch(url)).arrayBuffer();
	var data8 = new Uint8Array(data)
	buf.set(data8, ofs);
	return data8.length;
}

function wasmStr(buf, ofs) {
	if (!ofs)
		return null;
	var c, s = '';
	while ((c = buf[ofs++]))
		s += String.fromCharCode(c);
	return s;
}

async function loadAudio() {
	var imports = {
		env: {
			memory: new WebAssembly.Memory({initial: 4})
		}
	};
	var resp = await fetch('hmpweb/hmpweb.wasm');
	var lib = await WebAssembly.instantiate(await resp.arrayBuffer(), imports);
	//Object.assign(playerModule = {}, lib.instance.exports);
	playerModule = {};
	for (var x in lib.instance.exports)
		playerModule['_' + x] = lib.instance.exports[x];
	var mbuf = lib.instance.exports.memory.buffer;
	var buf = playerModule.HEAP8 = new Uint8Array(mbuf);
	playerModule.HEAP16 = new Int16Array(mbuf);
	var slen = await loadInto('descent.hmq', buf, playerModule._playerweb_get_data(0));
	var mlen = await loadInto('hammelo.bnk', buf, playerModule._playerweb_get_data(1));
	var dlen = await loadInto('hamdrum.bnk', buf, playerModule._playerweb_get_data(2));
	console.log({slen, mlen, dlen});
	var msg = playerModule._playerweb_play(slen, mlen, dlen, 0);
	console.log('play msg: ' + msg + ' ' + wasmStr(buf, msg));
	//playerModule._playerweb_init(48000);
	console.log('calling gen');
	//console.log('data: ' + playerModule._playerweb_gen(128));
	console.log('ret: ' + playerModule._playerweb_get_last_rc());
	fixAudio();
}

function libinit() {
	console.log('libinit');
	loadAudio();
	window.addEventListener('click', fixAudio);
	return;
	document.addEventListener("visibilitychange", function(ev) {
		console.log('vischange', ev);
	});
	window.addEventListener("focusin", function(ev) {
		Module._key_flush();
	});
	document.addEventListener('keydown', function(ev) {
		if (!audioContext)
			fixAudio();
		var keyCode = ev.keyCode;
		if (keyCode === 122) // preserve f11 (full screen)
			return;
		if (keyCode == 144) { // numlock
			Module._key_flush();
			return;
		}
		Module._on_key(keyCode, 1);
		//console.log('down ', ev.keyCode);
		ev.preventDefault();
		return false;
	});
	document.addEventListener('keyup', function(ev) {
		var keyCode = ev.keyCode;
		if (keyCode == 144) { // numlock
			Module._key_flush();
			return;
		}
		Module._on_key(ev.keyCode, 0);
		//console.log('up ', ev.keyCode);
		ev.preventDefault();
		return false;
	});

	document.body.addEventListener('click', function() {
		if (Module._get_function_mode() === 2)
			document.body.requestPointerLock && document.body.requestPointerLock();
		fixAudio();
	});
	document.body.addEventListener('mousedown', function(ev) {
		Module._on_mousebutton(ev.button, 1);
	});
	document.body.addEventListener('mouseup', function(ev) {
		Module._on_mousebutton(ev.button, 0);
	});
	document.body.addEventListener('mousemove', function(ev) {
		Module._on_mousemove(ev.movementX, ev.movementY);
	});
}


var musicVolume = 128 / 2, musicGain;
var digiVolume = 128 / 2, digiGain;
var audioContext, rate, samps, time;
var audioStarting;
function audioInit() {
  if (audioContext)
    audioContext.close();
  musicGain = null;
  digiGain = null;
  audioContext = new AudioContext();
  rate = 48000; //audioContext.sampleRate;
  samps = 48000 >> 3;
  time = 0;
  freeSources = [];
  audioStarting = true;
  if (audioContext.state == 'suspended') {
    fixAudio();
    return;
  }
  //playerweb._playerweb_init(rate);
  console.log('playerweb init done');
  audioInit2();
}

function audioInit2() {
  musicVolumeSet(musicVolume);
  digiGain = audioContext.createGain();
  digiGain.connect(audioContext.destination);
  digiVolumeSet(digiVolume);
}

function fillBuf(myBuffer) {
  let ch0 = myBuffer.getChannelData(0);
  let ch1 = myBuffer.getChannelData(1);
  let ofs = playerModule._playerweb_gen(samps * 2) >> 1;
  if (!ofs)
    throw new Error("playerweb: " + playerModule._playerweb_get_last_rc());
  //ofs >>= 1;
  let sndBuf = playerModule.HEAP16; //playerwebMem16s;
  //let sndBuf = playerwebMem16s.subarray(ofs, ofs + samps * 2);
  //console.log(sndBuf);
  for (var i = 0, p = ofs; i < samps; i++, p += 2) {
    ch0[i] = sndBuf[p] / 32768;
    ch1[i] = sndBuf[p + 1] / 32768;
  }
  //onsole.log(ofs);
  /*
  for (let sampleNumber = 0 ; sampleNumber < samps ; sampleNumber++) {
    ch0[sampleNumber] = generateSample(sampleNumber);
    ch1[sampleNumber] = generateSample(sampleNumber);
  }
  */
}
    
let freeBufs = [];
let bufCount = 0;
function nextBuf() {
  if (!audioContext)
    audioInit();
  if (!musicGain) {
    console.log('no audio');
    audioStarting && musicVolumeSet(musicVolume);
    return;
  }
  let myBuffer = freeBufs.length ? freeBufs.pop() : audioContext.createBuffer(2, samps, rate);
  fillBuf(myBuffer);
  let src = audioContext.createBufferSource();
  src.buffer = myBuffer;
  src.connect(musicGain);
  let startTime = time, lenTime = samps / rate;
  src.onended = function() { bufCount--; freeBufs.push(this.buffer); nextBuf(); if (audioContext.currentTime >= startTime + lenTime * 1.5) nextBuf(); }
  src.start(startTime);
  time += lenTime;
  bufCount++;
}

function playStart() {
	!window.audioContenxt && audioInit();
	if (!window.audioContext)
		return;
	if (musicGain) {
		musicGain.disconnect();
		musicGain = null;
	}
	console.log('playStart vol=', musicVolume);
	musicVolumeSet(musicVolume);
	//nextBuf();
	//nextBuf();
	//nextBuf();
	//nextBuf();
	//nextBuf();
}

var samples = new Array(256);
function getSampleBuf(num) {
	let buf = samples[num];
	if (buf)
		return buf;
	let len = Module._get_sample_size(num)
	buf = audioContext.createBuffer(1, len, 11025);
	let p = Module._get_sample_data(num);
	let heap = Module.HEAPU8;
	let data = buf.getChannelData(0);
	for (let i = 0; i < len; i++)
		data[i] = (heap[p + i] - 128) / 128;
	samples[num] = buf;
	return buf;
}
var freeSources = [];
var freePans = [];
var repeatSamples = [];
var repeatFree = [];
function playSample(num, pan, vol, repeat) {
	if (!digiGain)
		return;
	var srcNodes = freeSources.pop();
	var srcNode, gainNode, panNode;
	if (srcNodes) {
		[srcNode, gainNode, panNode] = srcNodes;
	} else {
		srcNode = audioContext.createBufferSource();
		gainNode = audioContext.createGain();
		panNode = audioContext.createStereoPanner();
		//srcNodes = [srcNode, gainNode, panNode];
		srcNode.connect(gainNode);
		gainNode.connect(panNode);
		panNode.connect(digiGain);
		//srcNode.onended = function() { freeSources.push(srcNodes); }
	}
	gainNode.gain.value = vol / (65536 * 2);
	panNode.pan.value = (pan - 32768) / 32768;
	srcNode.buffer = getSampleBuf(num);
	srcNode.loop = repeat;
	srcNode.start();
	if (!repeat)
		return -1;
	var idx = repeatFree.length ? repeatFree.pop() : repeatSamples.length;
	repeatSamples[idx] = [gainNode, panNode, srcNode];
	return idx;
}

function updateSample(idx, pan, vol) {
	var info = repeatSamples[idx];
	var gainNode = info[0], panNode = info[1], srcNode = info[2];
	gainNode.gain.value = vol / (65536 * 2);
	panNode.pan.value = (pan - 32768) / 32768;
}

function stopSample(idx) {
	var info = repeatSamples[idx];
	var gainNode = info[0], panNode = info[1], srcNode = info[2];
	srcNode.disconnect();
	gainNode.disconnect();
	panNode.disconnect();
	repeatSamples[idx] = null;
	repeatFree.push(idx);
}

function videoSetRes(w, h) {
	console.log('videoSetRes', w, h);
	width = w;
	height = h;
	ctx = null;
}

function musicVolumeSet(n) {
	console.log('musicVolumeSet ', n);
	musicVolume = n;
	var vol = n / 128;
	if (!n) {
		musicGain && musicGain.disconnect();
		musicGain = null;
	} else {
		if (!musicGain) {
			if (!audioContext || audioContext.state == 'suspended')
				return;
			audioStarting = false;
			musicGain = audioContext.createGain();
			musicGain.gain.value = vol;
			musicGain.connect(audioContext.destination);
			time = audioContext.currentTime;
			nextBuf();
			nextBuf();
			console.log('started');
		} else
			musicGain.gain.setTargetAtTime(vol, audioContext.currentTime, 0.1);
	}
}

function digiVolumeSet(n) {
	console.log('digiVolumeSet ', n);
	digiVolume = n;
	if (digiGain)
		digiGain.gain.value = n / 128;
}

function playAd(wakeUp) {
	var div = document.createElement('div');
	var allow2;
	function done1() { allow2 = true; }
	function done2() { if (allow2) done(); }
	function done() {
		if (!wakeUp)
			return;
		window.removeEventListener('keydown', done1, true);
		window.removeEventListener('keyup', done2, true);
		div.remove();
		console.log('waking up');
		wakeUp();
		wakeUp = null;
	}
	console.log('playAd');
	div.innerHTML = '<div tabindex="0" style="background:#000;position:absolute;top:0;left:0;right:0;bottom:0;display:flex;justify-content:center"><video autoplay style="flex:1;object-fit:contain"><source src="descent.mp4"></video></div>';
	div.firstChild.onkeypress = div.firstChild.onclick = div.querySelector('video').onended = done;
	document.body.appendChild(div);
	window.addEventListener('keydown', done1, true);
	window.addEventListener('keyup', done2, true);
}
function playStop() {
	if (musicGain) {
		musicGain.disconnect();
		musicGain = null;
	}
}

function saveFile(fn, a) {
	var rawLength = a.length;
	var b = new Array(rawLength);
	for (var i = 0; i < rawLength; i++)
		b[i] = String.fromCharCode(a[i]);
	localStorage.setItem(fn, window.btoa(b.join("")));
}

function loadFile(fn) {
	var val = localStorage.getItem(fn);
	if (!val)
		return;
	var raw = window.atob(val);
	var rawLength = raw.length;
	var array = new Uint8Array(new ArrayBuffer(rawLength));
	for (var i = 0; i < rawLength; i++)
		array[i] = raw.charCodeAt(i);
	return array;
}

function listFiles() {
	return Object.keys(localStorage);
}

function loadFilenames(buf, fnsize, fnmax, filespec, stripext, host) {
	var mem = Module.HEAPU8, p = filespec, a = [], c;
	while ((c = mem[p++]))
		a.push(String.fromCharCode(c));
	var filespecstr = a.join("");
	console.log('loadFilenames ' + filespecstr);
	var re = new RegExp(filespecstr.substr(1).replace(new RegExp("\\W", "g"), "\\$&") + "$", "i");
	var num = 0;
	p = buf;
	for (let fn of host ? FS.readdir('/') : listFiles()) {
		console.log(fn + ' ' + re.test(fn));
		if (re.test(fn) && fn.length < fnsize) {
			if (stripext)
				fn = fn.substr(0, fn.lastIndexOf('.'));
			for (var l = fn.length, i = 0; i < l; i++)
				mem[p + i] = fn.charCodeAt(i);
			mem[p + l] = 0;
			p += fnsize;
			num++;
		}
	}
	console.log(num);
	return num;
}
function x2(n) { return n < 16 ? '0' + n.toString(16) : n.toString(16); }
function memHex(a, p, len) { var s = ''; while (len--) s += x2(a[p++]); return s; }
var p2p;
function strBefore(s, pat) { var i = s.indexOf(pat); return i === -1 ? s : s.substr(0, i); }
function getSignalingServer() {
	if (location.hostname === 'localhost')
		return 'ws://localhost:8080';
	return 'wss://' + location.hostname;
	//return 'ws' + location.protocol.substr(4) + '//' + strBefore(location.host, ':') + ':8590';
}
var ICE_SERVERS = [{url:"stun:stun.l.google.com:19302"}];
var netMsgs, netPeers, netQueuedAll, netOpen;
function netInit(node) {
	if (p2p)
		return;
	var mem = Module.HEAPU8;
	p2p = new P2P({server:getSignalingServer(), iceServers:ICE_SERVERS, data:memHex(mem, node, 6)});
	netMsgs = [];
	netPeers = {};
	netQueuedAll = null;
	netOpen = false;
	p2p.onopen = function() { netQueuedAll && p2p.sendAll(netQueuedAll); netQueuedAll = null; netOpen = true; }
	p2p.onmessage = function(msg, peer_id, data) {
		msg = new Uint8Array(msg);
		netPeers[data] = peer_id;
		netMsgs.push([msg, data]);
		console.log('got ', peer_id, data, msg);
	};
	p2p.start();
}
function netSend(node, data, len) {
	if (!p2p || !netPeers)
		return;
	var mem = Module.HEAPU8;
	var peer_id = netPeers[memHex(mem, node, 6)];
	if (!peer_id) {
		console.log('unknown peer ' + memHex(mem, node, 6));
		return;
	}
	console.log('sending to ' + memHex(mem, node, 6), peer_id, mem.subarray(data, data + len));
	p2p.send(peer_id, mem.subarray(data, data + len));
}
function netSendAll(data, len) {
	var packet = Module.HEAPU8.subarray(data, data + len);
	p2p && console.log('broadcast ', packet);
	p2p && p2p.sendAll(packet);
	if (!p2p || !netOpen)
		netQueuedAll = packet;
}
function netReceive(data, maxLen) {
	if (!netMsgs)
		return 0;
	var msg = netMsgs.shift();
	if (!msg)
		return 0;
	var len = msg[0].length;
	if (len > maxLen)
		msg[0] = msg[0].subarray(0, len = maxLen);
	var mem = Module.HEAPU8;
	mem.set(msg[0], data);
	console.log('received ' + len);
	return len;
}
