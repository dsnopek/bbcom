
Drupal.behaviors.bibliobird_flashcards = function (context) {
    var link = $('<a id="bibliobird-flashcards-show-answer" href="#"></a>')
        .text(Drupal.t('Show answer'))
        .click(function (evt) {
            $('#bibliobird-flashcards-flashcard-answer').css('visibility', 'visible');
            $('#bibliobird-flashcards-ease-buttons').show();
            link.remove();
            evt.preventDefault();
        })
        .insertAfter($('#bibliobird-flashcards-flashcard', context));

    // setup our tabs
    $('#bibliobird-flashcards-study-options', context).tabs();

    // should use .unwrap() but this is jquery 1.2
    $('#tab-new-cards fieldset, #tab-reviews fieldset', context).each(function () {
        var n = $(this);
        $('legend', n).remove();
        n.replaceWith(n.contents());
    });
};

