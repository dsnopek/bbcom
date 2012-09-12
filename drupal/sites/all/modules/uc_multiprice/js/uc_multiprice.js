$(document).ready( function() {
	// unbind any functions and submit form onchange													
	$('#edit-panes-delivery-delivery-country').unbind('change').trigger('change').change( function(){$('#uc-cart-checkout-form').submit();});
});