function P2P(options) {
	var p2p = this;
	this.options = options;
	var peers = {};
	var ws, connectTimer;
	function startConnect() {
		clearTimeout(connectTimer);
		connect();
		connectTimer = setTimeout(startConnect, 5000);
	}
	function mkErr(func) {
		return function(msg) { console.error(func, msg); };
	}
	function clearPeers() {
		for (var peer_id in peers) {
			var peer = peers[peer_id];
			peer.chan && peer.chan.close();
			peer.conn && peer.conn.close();
			delete peers[peer_id];
		}
	}
	function acceptDesc(peer_id, desc) {
		peers[peer_id].conn.setLocalDescription(desc, function() {
			ws.send(JSON.stringify({type:'relaySessionDescription', peer_id:peer_id, session_description:desc}));
			console.log(peer_id, 'sent local desc', desc);
		}, mkErr('setLocalDescription'));
	}
	function createOffer(conn, peer_id) {
		conn.createOffer(function(desc) { acceptDesc(peer_id, desc); }, mkErr('createOffer'));
	}
	function acceptChan(peer_id, chan) {
		console.log(peer_id, 'got chan');
		chan.onopen = function() { peers[peer_id].chan = chan; console.log(peer_id, 'open channel'); }
		chan.onclose = function() { peers[peer_id] && delete peers[peer_id].chan; }
		chan.onmessage = function(ev) { p2p.onmessage && p2p.onmessage(ev.data, peer_id, peers[peer_id].data); };
		chan.binaryType = 'arraybuffer';
		p2p.onopen && p2p.onopen(peer_id, peers[peer_id].data);
	}
	var funcs = {};
	funcs.addPeer = function(msg) {
		var peer_id = msg.peer_id;
		var peer = peers[peer_id] = {data:msg.data};
		var conn = peer.conn = new RTCPeerConnection({iceServers:options.iceServers});
		conn.onicecandidate = function() {
			console.log(peer_id, 'on ice', event.candidate);
			event.candidate && ws.send(JSON.stringify({type:'relayICECandidate', peer_id:peer_id, ice_candidate:event.candidate}));
		}
		conn.ondatachannel = function(ev) { console.log(peer_id, 'accepting chan'); acceptChan(peer_id, ev.channel); };
		msg.should_create_offer && acceptChan(peer_id, conn.createDataChannel('msg', {ordered:false}));
		msg.should_create_offer && createOffer(conn, peer_id);
	}
	funcs.removePeer = function(msg) {
		var peer_id = msg.peer_id;
		var peer = peers[peer_id];
		if (!peer)
			return;
		peer.conn && peer.conn.close();
		delete peers[peer_id];
	}
	funcs.sessionDescription = function(msg) {
		var peer_id = msg.peer_id;
		var peer = peers[peer_id];
		if (!peer)
			return;
		var desc = new RTCSessionDescription(msg.session_description);
		console.log(peer_id, 'have remote desc');
		peer.conn.setRemoteDescription(desc, function() {
			msg.session_description.type == 'offer' && peer.conn.createAnswer(function(ldesc) { acceptDesc(peer_id, ldesc); }, mkErr('createAnswer'));
		}, mkErr('setRemoteDescription'));
	}
	funcs.iceCandidate = function(msg) {
		var peer = peers[msg.peer_id];
		if (!peer)
			return;
		peer.conn.addIceCandidate(new RTCIceCandidate(msg.ice_candidate));
	}
	function connect() {
		ws && ws.close();
		ws = new WebSocket(options.server);
		ws.onopen = function() { 
			clearTimeout(connectTimer);
			clearPeers();
			var msg = {type:'join',channel:'main',data:options.data};
			console.log('signal send join', msg);
			ws.send(JSON.stringify(msg)); 
		}
		ws.onclose = function() { ws = null; }
		ws.onmessage = function(ev) {
			console.log('signal', ev.data);
			var msg = JSON.parse(ev.data);
			msg.type != 'addPeer' && !peers[msg.peer_id] && console.log('unknown peer ', msg.peer_id);
			funcs[msg.type](msg);
		};
	}
	function send(dest, msg) {
		var peer = peers[dest];
		peer && peer.chan && peer.chan.send(msg);
	}
	this.start = startConnect;
	this.send = send;
	this.sendAll = function(msg) { for (var dest in peers) send(dest, msg); }
}
