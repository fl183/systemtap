#! /usr/bin/perl 

sub bitset {
    my $num = shift;
    my $bit = shift;
    return ($num & (1 << $bit));
}

sub gensn {
    my $permutation = shift;
    my $arity = shift;

    my $SN = "";
    for (0..$arity-1) { $SN .= bitset($permutation,$_) ? "S" : "N"; }

    print "#define STAP_MARK" . ($arity>0?"_":"") . $SN . "(n";
    for (0..$arity-1) { print ",a" . (($_)+1); }
    print ") do { \\\n";
    print "  static void (*__systemtap_mark_##n##_" . $SN . ")(";
    for (0..$arity-1) { if ($_) { print ",";} 
                        print bitset($permutation,$_) 
                               ? "const char *"
                               : "int64_t"; }
    print "); \\\n";
    print "  if (unlikely (__systemtap_mark_##n##_" . $SN . ")) \\\n";
    print "    (void) (__systemtap_mark_##n##_" . $SN . "(";
    for (0..$arity-1) { if ($_) { print ",";} 
                        print "(a" . (($_)+1) . ")"; }
    print ")); \\\n";
    print "} while (0)\n\n";
}

sub permute {
    my $arity = shift;
    for (0 .. (1<<$arity)-1) {
        &gensn ($_, $arity);
    }
}


die "give me one number" unless $#ARGV == 0;

print "/* Generated by '$0 @ARGV' on " . gmtime() . " */\n\n";

for (0 .. $ARGV[0]) {
    &permute($_);
}
