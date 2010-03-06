<?php

class Base32 {
  // See http://www.crockford.com/wrmg/base32.html
  private $charset = '0123456789ABCDEFGHJKMNPQRSTVWXYZ';

  public function toDec($str) {
    // pre-process the string a little bit
    $str = strtoupper($str);
    $str = str_replace('O','0',$str);
    $str = str_replace(array('I','L'),'1',$str);
    $str = strrev($str);

    $dec = 0;
    for($i = 0; $i < strlen($str); $i++) {
      $weight = ($i > 0 ? $i * 32 : 1);
      $dec += $weight * strpos($this->charset, $str[$i]);
    }
    return $dec;
  }
  
  public function fromDec($num) {
    if ($num == 0) return '0';

    $s = '';
    while ($num > 0) {
      $remainder = $num % 32;
      $num = intval($num / 32);

      $s = ($this->charset[$remainder] . $s);
    }

    return $s;
  }
}

