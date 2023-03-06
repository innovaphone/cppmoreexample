
/// <reference path="../../web1/lib1/innovaphone.lib1.js" />
/// <reference path="../../web1/appwebsocket/innovaphone.appwebsocket.Connection.js" />
/// <reference path="../../web1/ui1.lib/innovaphone.ui1.lib.js" />

var innovaphone = innovaphone || {};
innovaphone.cppmoreexampleAdmin = innovaphone.cppmoreexampleAdmin || function (start, args) {
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

    var counter = this.add(new innovaphone.ui1.Div("position:absolute; left:0px; width:100%; top:calc(50% - 50px); font-size:100px; text-align:center", "-"));
    var app = new innovaphone.appwebsocket.Connection(start.url, start.name);
    app.checkBuild = true;
    app.onconnected = app_connected;

    function app_connected(domain, user, dn, appdomain) {
        var src = new app.Src(update);
        src.send({ mt: "MonitorCount" });

        that.add(new innovaphone.ui1.Div(null, null, "button")).addTranslation(texts, "reset").addEvent("click", function () {
            app.sendSrc({ mt: "ResetCount" }, function (msg) {
                counter.addHTML("" + msg.count);
            });
        });

        function update(msg) {
            if (msg.mt == "MonitorCountResult" || msg.mt == "UpdateCount") {
                counter.addHTML("" + msg.count);
            }
        }
    }
}

innovaphone.cppmoreexampleAdmin.prototype = innovaphone.ui1.nodePrototype;
