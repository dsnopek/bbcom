
function uc_multiprice_dynamic(field_id, field, countryrole) {

	var field = field.replace('_', '-');
	var _return = $("#edit-multiprice-multiprices-"+countryrole+"-"+field_id+"-"+field+"");
	var _default = $("#edit-multiprice-multiprices-0-"+field);
	var dynamic_button = $("#edit-multiprice-multiprices-"+countryrole+"-"+field_id+"-edit-"+field);
	var enabled = $("#edit-multiprice-multiprices-"+countryrole+"-"+field_id+"-dynamic-"+field+"-enabled");
	
	//check if enabled otherwise quit
	if (enabled.attr("checked")) {
		$(dynamic_button).addClass('dynamic_on');
		_return.attr("disabled", "disabled");
		$(_return).addClass('disabled');
	}else{
		$(dynamic_button).removeClass('dynamic_on');
		_return.attr("disabled", "");
		$(_return).removeClass('disabled');
		return;
	}

	// get operators and values
	var operator 		= $("#edit-multiprice-multiprices-"+countryrole+"-"+field_id+"-dynamic-"+field+"-operator-value");
	var operator_1 	= $("#edit-multiprice-multiprices-"+countryrole+"-"+field_id+"-dynamic-"+field+"-1-operator");
	var value_1			= $("#edit-multiprice-multiprices-"+countryrole+"-"+field_id+"-dynamic-"+field+"-1-value");
	var operator_2	= $("#edit-multiprice-multiprices-"+countryrole+"-"+field_id+"-dynamic-"+field+"-2-operator");
	var value_2 		= $("#edit-multiprice-multiprices-"+countryrole+"-"+field_id+"-dynamic-"+field+"-2-value");

	// do some changing
	if (operator_1.val() == 'default') {
		value_1.val(_default.val());
		value_1.attr("disabled", "disabled");
		$(value_1).addClass('disabled');
	}else{
		value_1.attr("disabled");
		$(value_1).removeClass('disabled');
	}
	
	if (operator_2.val() == 'default') {
		value_2.val(_default.val());
		value_2.attr("disabled", "disabled");
		$(value_2).addClass('disabled');
	}else{
		value_2.attr("disabled");
		$(value_2).removeClass('disabled');
	}
	
	if (operator_1.val() == 'custom') {
		value_1.attr("disabled", '');
	}
	if (operator_2.val() == 'custom') {
		value_2.attr("disabled", '');
	}
	
	// dynamiculations
	if (operator.val() == 'x') {
		result = parseFloat(value_1.val()) * parseFloat(value_2.val());
	}
	if (operator.val() == '-') {
		result = parseFloat(value_1.val()) - parseFloat(value_2.val());
	}
	if (operator.val() == '+') {
		result = parseFloat(value_1.val()) + parseFloat(value_2.val());
	}
	if (operator.val() == '/') {
		result = parseFloat(value_1.val()) / parseFloat(value_2.val());
	}
	
	// return
	_return.val(result);	
}

function uc_multiprice_dynamic_recalculate(field) {
	jQuery.each(Drupal.settings.uc_multiprice.country, function() {
			uc_multiprice_dynamic(this, field, 'country');																																
	});
	jQuery.each(Drupal.settings.uc_multiprice.role, function() {
			uc_multiprice_dynamic(this, field, 'role');																																
	});
}

Drupal.behaviors.uc_multiprice_dynamic = function (context) {
	$('.popup').hide();
	
	uc_multiprice_dynamic_recalculate('list-price');
	uc_multiprice_dynamic_recalculate('sell-price');
	uc_multiprice_dynamic_recalculate('cost')

	$('.dynamic').click(function(){
		var id = $(this).attr('id');
		
		$('.popup:not(#'+ id +'-dynamic)').hide();
		$('.dynamic:not(#'+ id +')').parents("td").removeClass("dynamic_tab"); 
		
		$('#'+ id +'-dynamic').toggle();
		$(this).parents("td").toggleClass("dynamic_tab"); 
		
		return false;
	});

	// add listeners to default
	$("#edit-multiprice-multiprice-0-list-price").change(function(){
			uc_multiprice_dynamic_recalculate('list-price');
	});
	$("#edit-multiprice-multiprice-0-sell-price").change(function(){
			uc_multiprice_dynamic_recalculate('sell-price');
	});
	$("#edit-multiprice-multiprice-0-cost").change(function(){
			uc_multiprice_dynamic_recalculate('cost');
	});
	
	// undisable all fields on submit (fapi bug?)
	$("#node-form").submit(function() {
		$("#uc_multiprice_table input:text").attr("disabled", "");
  });

};
