
Drupal.behaviors.bibliobird_flashcards = function (context) {
    var link = $('<a href="#"></a>')
        .text(Drupal.t('Show answer'))
        .click(function (evt) {
            $('#bibliobird-flashcards-title').text(Drupal.t('Answer'));
            $('#bibliobird-flashcards-flashcard-question').hide();
            $('#bibliobird-flashcards-flashcard-answer').show();
            $('#bibliobird-flashcards-ease-buttons').show();
            link.remove();
            evt.preventDefault();
        })
        .insertAfter($('#bibliobird-flashcards-flashcard', context));
};

