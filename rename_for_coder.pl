#!/usr/bin/perl

opendir(DH, ".") or die("opendir");
@f=readdir(DH);
closedir(DH);

$count = 1;

foreach $f (@f) {
    next unless (-f $f);
    if ($f =~ /^(.*?).raw$/) {
       # $n=ord($1)-64;
       # $n+= 12 * (($2)-1) ;
       # $new=sprintf("%03d_%s", $n, $f);
	$new_filename = "sound$count.raw";
      rename($f, $new_filename) unless (-e $new_filename);
	print($f . " " . $new_filename . "\n");

	$count++;
    }
}