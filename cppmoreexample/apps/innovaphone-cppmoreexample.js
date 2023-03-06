
/// <reference path="../../web1/lib1/innovaphone.lib1.js" />
/// <reference path="../../web1/appwebsocket/innovaphone.appwebsocket.Connection.js" />
/// <reference path="../../web1/ui1.lib/innovaphone.ui1.lib.js" />

var innovaphone = innovaphone || {};
innovaphone.cppmoreexample = innovaphone.cppmoreexample || function (start, args) {
    this.createNode("body");
    var that = this;

    var colorSchemes = {
        dark: {
            "--bg": "#191919",
            "--button": "#303030",
            "--text-standard": "#f2f5f6",
        },
        light: {
            "--bg": "white",
            "--button": "#e0e0e0",
            "--text-standard": "#4a4a49",
        }
    };
    var schemes = new innovaphone.ui1.CssVariables(colorSchemes, start.scheme);
    start.onschemechanged.attach(function () { schemes.activate(start.scheme) });

    var texts = new innovaphone.lib1.Languages(innovaphone.cppmoreexampleTexts, start.lang);
    start.onlangchanged.attach(function () { texts.activate(start.lang) });

    // create UI container before AppWebsocket connection is created
    var lettersContainer = this.add(new innovaphone.ui1.Div("display: flex; flex-wrap: wrap;", ""));

    var app = new innovaphone.appwebsocket.Connection(start.url, start.name);
    app.checkBuild = true;
    app.onconnected = app_connected;

    function app_connected(domain, user, dn, appdomain) {
        // send the initial request
        app.sendSrcMore({ api: "Alphabet", mt: "GetLetters", limit: 10 }, callback);
    }

    function callback(msg) {
        // process response, add letters to UI container
        if ("letters" in msg && Array.isArray(msg.letters)) msg.letters.forEach(function (letter) {
            lettersContainer.add(new innovaphone.ui1.Div("font-size:30px; margin: 10px; text-align:center", letter));
        });
    }
};

innovaphone.cppmoreexample.prototype = innovaphone.ui1.nodePrototype;