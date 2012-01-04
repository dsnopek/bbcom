
function bbcom_metrics_init() {
  // Copied from the mixpanel docs
  // NOTE: we removed the 'var' on purpose, so this is a global.
  mpq=[];mpq.push(["init",Drupal.settings.bbcom_metrics.mixpanel_token]);(function(){var b,a,e,d,c;b=document.createElement("script");b.type="text/javascript";b.async=true;b.src=(document.location.protocol==="https:"?"https:":"http:")+"//api.mixpanel.com/site_media/js/api/mixpanel.js";a=document.getElementsByTagName("script")[0];a.parentNode.insertBefore(b,a);e=function(f){return function(){mpq.push([f].concat(Array.prototype.slice.call(arguments,0)))}};d=["init","track","track_links","track_forms","register","register_once","identify","name_tag","set_config"];for(c=0;c<d.length;c++){mpq[d[c]]=e(d[c])}})();

  // Runs commands from bbcom_metrics
  (function(){var i,c=Drupal.settings.bbcom_metrics.commands;for(i=0;i<c.length;i++){mpq[c[i][0]].apply(mpq,c[i][1])}})();
}

