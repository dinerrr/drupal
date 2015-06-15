(function($) {
    Drupal.behaviors.testshelper = {
        attach : function(context, settings) {
            $('.open-all-links').click(function(e) {
                e.preventDefault();
                Drupal.settings.testshelper.forEach(function(entry) {
                    window.open(entry.url_absolute);
                });
            });
        }
    };
})(jQuery);
