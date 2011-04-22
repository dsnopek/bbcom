<?php 

//+ Jonas Raoni Soares Silva
//@ http://jsfromhell.com
class Language{
	private static $language = null;

	public static function get(){
		if (self::$language === NULL) {
      self::init();
    }
		return self::$language;
	}
	public static function getBestMatch($langs = array(), $default=NULL){
		foreach($langs as $n => $v)
			$langs[$n] = strtolower($v);
		$r = array();
		foreach(self::get() as $l => $v){
			($s = strtok($l, '-')) != $l && $r[$s] = 0;
			if(in_array($l, $langs))
				return $l;
		}
		foreach($r as $l => $v)
			if(in_array($l, $langs))
				return $l;
		return $default;
	}
	private static function init(){
		if(self::$language !== null)
			return;
		if(($list = strtolower($_SERVER['HTTP_ACCEPT_LANGUAGE']))){
			if(preg_match_all('/([a-z]{1,8}(?:-[a-z]{1,8})?)(?:;q=([0-9.]+))?/', $list, $list)){
				self::$language = array_combine($list[1], $list[2]);
				foreach(self::$language as $n => $v)
					self::$language[$n] = +$v ? +$v : 1;
				arsort(self::$language);
			}
		}
		else
			self::$language = array();
	}
}

$supported = array('en', 'pl');
$lang = Language::getBestMatch($supported, $supported[0]);
$url = 'http://'. $lang .'.bibliobird.com'. $_SERVER['PATH_INFO'];
if (!empty($_SERVER['QUERY_STRING'])) {
  $url .= '?'. $_SERVER['QUERY_STRING'];
}
header('Location: '. $url, TRUE, 302);

