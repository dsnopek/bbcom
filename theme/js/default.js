
Drupal.behaviors.bibliobird = function (context) {
    /* JavaScript for the language switcher */
    var form = $('#lingwoorg-theme-language-switcher-form', context);
    if (form) {
        $('#edit-my-language-select', form).change(function (evt) { 
            document.location.href=evt.target.options[evt.target.selectedIndex].value;
        });
    }

    /* Fix for IE6 flickering images issue. */
    try {
        document.execCommand("BackgroundImageCache", false, true);
    } catch(e) { };

    /* Warn the user about remote links. */
    $("a[@rel='remote']").click(function (evt) {
        if (!confirm(Drupal.t('This text is located on an external site.  Do you want to continue?'))) {
            evt.preventDefault();
            return false;
        }
    });
};

