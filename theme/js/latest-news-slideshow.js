$(document).ready(function () {
    $('#latest-news-slideshow').cycle({
        fx: 'fade',
        timeout: 0,
        pager: '#latest-news-slideshow-pager',
        pagerAnchorBuilder: function (idx, slide) {
            return '<li><a href="#">'+$('.slide-text-title', slide).text()+'</a></li>';
        },
    });
});

