
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
};

