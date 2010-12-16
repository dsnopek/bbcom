
Drupal.behaviors.lingwoorg_flashcards = function (context) {
    var link = $('<a href="#"></a>')
        .text(Drupal.t('Show answer'))
        .click(function (evt) {
            $('#lingwoorg-flashcards-title').text(Drupal.t('Answer'));
            $('#lingwoorg-flashcards-flashcard-question').hide();
            $('#lingwoorg-flashcards-flashcard-answer').show();
            $('#lingwoorg-flashcards-ease-buttons').show();
            link.remove();
            evt.preventDefault();
        })
        .insertAfter($('#lingwoorg-flashcards-flashcard', context));
};

