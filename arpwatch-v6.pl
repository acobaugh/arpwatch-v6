#!/usr/bin/env perl

use warnings;
use strict;

use Getopt::Long;

sub usage {
	print "Usage: arpwatch-v6.pl [--help] [-h]\n";
	exit 1;
}

my %o = ();
Getopt::Long::Configure("bundling");
GetOptions(\%o, 
	'h',
	'help'
) or usage();

if ($o{'h'}) {
	usage();
	exit 1;
}
if ($o{'help'}) {
	exec('perldoc', '-T', $0);
	exit 1;
}

__END__

=head1 NAME

arpwatch-v6.pl - An reimplementation of arpwatch in Perl with support for IPv6 NDP

=head1 SYNOPSIS

arpwatch-v6.pl [--help] [-h]

=head1 OPTIONS

Required:

Optional:

=over

=item B<--help>

Prints this full help message.

=item B<-h>

Prints the short usage summary.

=back

=head1 SEE ALSO

Net::Pcap(3), pcap(3)

L<http://github.com/phalenor/arpwatch-v6>

=head1 AUTHOR

Andy Cobaugh <phalenor@gmail.com>

=cut

