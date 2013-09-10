
Drupal.behaviors.bibliobird_flashcards = function (context) {
    var link = $('<a id="bibliobird-flashcards-show-answer" href="#"></a>')
        .text(Drupal.t('Show answer'))
        .click(function (evt) {
            $('#bibliobird-flashcards-flashcard-question').hide();
            $('#bibliobird-flashcards-flashcard-answer').show();
            $('#bibliobird-flashcards-ease-buttons').show();
            link.remove();
            evt.preventDefault();
        })
        .insertAfter($('#bibliobird-flashcards-flashcard', context));

    function updateStudyOptions() {
      var type = '' + $('#edit-study-options-type').val();
      $('.bibliobird-flashcards-study-options-toggle').hide();
      $('.bibliobird-flashcards-study-options-' + type).show();
    }

    // Hide/show fields depending on current review type
    $('#edit-study-options-type', context).change(updateStudyOptions);
    updateStudyOptions();
};

