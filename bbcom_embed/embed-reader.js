(function(){var m;(function(){function c(d){var b=j[d];if(b)return b;if(!i[d])throw new Error("Module "+d+" not found");for(var k=i[d],a=k.deps||(k.length?["require","exports","module"]:[]),e=j[d]={},g=0;g<a.length;g++){var h=a[g];switch(h){case "require":h=function(f){if(f.charAt(0)==="."){for(f=d.substring(0,d.lastIndexOf("/")+1)+f;l!==f;){var l=f;f=f.replace(/\/[^\/]*\/\.\.\//,"/")}f=f.replace(/\/\.\//g,"/")}return c(f)};break;case "exports":h=e;break;case "module":b=h={exports:e};break;default:h=
c(h)}a[g]=h}e=k.apply(this,a);if(b&&b.exports!=j[d])e=b.exports;if(e)return j[d]=e;return j[d]}var i={},j={};m=function(d,b){for(var k=[],a=0;a<d.length;a++)k.push(c(d[a]));b.apply(c,k)};m.def=function(d,b,k){if(typeof b=="function")i[d]=b;else(i[d]=k).deps=b}})();m.def("jquery",[],function(){return window.jQuery});m.def("jQuery",[],function(){return window.jQuery});m.def(["jquery","lingwo_dictionary/annotation/Reader2"],function(c,i){function j(a,e,g){var h;for(h in e)if(!g||typeof a[h]=="undefined")a[h]=
e[h]}function d(a){var e=b.url.replace("<lang>",b.lang);if(a)return e+"/"+a;return e}var b={},k={url:"http://<lang>.bibliobird.com",lang:"en",showLinks:true,ga:true,pageUrlRemove:"#",getPageUrl:function(){var a=""+window.location.href;switch(b.pageUrlRemove){case "?":a=a.split("?")[0];case "#":a=a.split("#")[0]}return a}};if(typeof window.BiblioBird!="undefined")b=window.BiblioBird;j(b,k,true);j(b,{initialized:false,contentAreas:[],embedWindow:null,embedIframe:null,embedWindowShown:false,username:null,
start:function(){var a=this;c(".bibliobird-content").each(function(){a.contentAreas.push(new ContentArea(this))})},initialize:function(a){function e(){var f=(c(window).width()-400)/2,l=(c(window).height()-200)/2;g.css({top:l,left:f})}if(!this.initialized){var g=c('<div class="clear-block"><div class="bibliobird-embed-titlebar"><span class="bibliobird-embed-title">BiblioBird</span> <a href="#" class="bibliobird-embed-close">close</a></div></div>'),h=c('<iframe height="100%" width="100%" border="0" id="bibliobirdEmbedIframe" name="bibliobirdEmbedIframe"></iframe>').appendTo(g);
g.css({width:400,height:200,position:"fixed",border:"1px solid black",background:"white",display:"none"}).appendTo(c("body"));c(".bibliobird-embed-close",g).click(function(){b.hideEmbedWindow();return false});c(window).resize(e);setTimeout(e,0);i.setup({layout:"popup",skipHoverEvents:true});c("body").click(function(f){b.hideEmbedWindow();return i.handleClick(f.target)});j(this,{embedWindow:g,embedIframe:h,username:a,initialized:true})}},receiveMessage:function(a){if(a.indexOf("#login-successful")==
0){a=a.split(":");if(a[1])this.username=a[1];this.hideEmbedWindow()}},openEmbedWindow:function(a){if(this.embedWindow!==null&&!this.embedWindowShown){this.embedIframe.attr("src",a);this.embedWindow.show();this.embedWindowShown=true}},hideEmbedWindow:function(){if(this.embedWindow!==null&&this.embedWindowShown){this.embedWindow.hide();this.embedWindowShown=false;this.refreshAll()}},logout:function(){var a=this;c.ajax({url:d("remote/logout"),dataType:"jsonp",success:function(){a.username=null;a.refreshAll()}})},
refreshAll:function(){c.each(this.contentAreas,function(){this.refresh()})}});i.onLoad=function(a){a=(a.attr("data-entry")||"").split("#");var e=a[0],g=a[1];i.setBubbleLoading();c.ajax({url:d("embed-reader/lookup_entry"),dataType:"jsonp",data:{hash:e,lang:b.lang},success:function(h){i.setBubbleContent(h.content);b.ga&&typeof window._gaq!="undefined"&&_gaq.push(["_trackEvent","BiblioBird","Anonymous Lookup",b.lang]);c(".node",i.contentNode).removeClass("clear-block").removeAttr("id");c("ul.links.inline",
i.contentNode).remove();g&&c(".lingwo-sense-id-"+g,i.contentNode).addClass("selected");c("a",i.contentNode).each(function(f,l){f=l.href;var n;if(f){n=window.location.protocol+"//"+window.location.host;l.href=f.replace(n,d());l.target="_blank"}})}})};ContentArea=function(a){this.node=a;this.url=c(a).attr("data-url")||b.getPageUrl();this.teaser=c(a).attr("data-teaser")=="true";this.links=c('<div class="bibliobird-links"></div>').insertBefore(a);this.data={};this.lookupContent()};j(ContentArea.prototype,
{lookupContent:function(){var a=this;c.ajax({url:d("embed-reader/lookup_content"),dataType:"jsonp",data:{url:this.url},success:function(e){b.initialize(e.username);a.data=e;a.rebuildLinks();if(e.content){a.node.innerHTML=e.content;i.setupHoverEvents()}}})},rebuildLinks:function(){if(b.showLinks){var a=this.links,e=this.data;a.html("");if(b.username){a.append("Logged into BiblioBird as "+b.username+" ");a.append(c("<a></a>").html("Logout").attr("href",d("logout")).click(function(){b.logout();return false}))}else{a.append("Not logged into BiblioBird ");
a.append(c("<a></a>").html("Login").attr("href",d("remote/login")+(b.localRelayUrl?"?relay="+b.localRelayUrl:"")).click(function(g){b.openEmbedWindow(g.target.href);return false}));a.append(" ");a.append(c("<a></a>").html("Join BiblioBird").attr({href:d("user/register"),target:"_blank"}))}a.append(" ");e.not_found&&a.append(c("<a></a>").html("Add to Bibliobird").attr({href:d("node/add/content?remote_url="+escape(this.url)),target:"_blank"}))}},refresh:function(){this.rebuildLinks()}});return window.BiblioBird=
b});m(["lingwo_dictionary/annotation/Embed"],function(c){c.start()})})();
