#                              -*- Mode: Perl -*- 
# $Basename: WAIT.pm $
# $Revision: 1.6 $
# Author          : Ulrich Pfeifer
# Created On      : Fri Jan 31 11:30:46 1997
# Last Modified By: Ulrich Pfeifer
# Last Modified On: Thu Mar 23 21:19:20 2000
# Language        : CPerl
# Update Count    : 145
# Status          : Unknown, Use with caution!
# 
# (C) Copyright 1997, Ulrich Pfeifer, all rights reserved.
# 
# 

package CPAN::WAIT;
use ExtUtils::MakeMaker; # MM->catfile

# try to avoid 'use'ing CPAN.pm
# code stolen from CPAN::Config::load ;

eval {require CPAN::Config;};       # We eval because of some
                                    # MakeMaker problems
unless ($CPAN::dot_cpan++){
  unshift @INC, MM->catdir($ENV{HOME},".cpan");
  eval {require CPAN::MyConfig;};   # where you can override
                                    # system wide settings
  shift @INC;
}
 


require WAIT::Client;
require FileHandle;
use vars qw($VERSION $DEBUG $TIMEOUT);

# $Format: "\$\V\E\R\S\I\O\N = '$ModuleVersion$';"$ MM_Unix bug
$VERSION = '0.27';
$TIMEOUT = 20;                  # Set this to some larger value if you
                                # have a slow connection.

sub _open_connection () {
  my ($host, $port, $con);
  
  # Make sure that there is a wait server to try
  unless ($CPAN::Config->{'wait_list'}) {
    $CPAN::Config->{'wait_list'} = ['wait://ls6.informatik.uni-dortmund.de'];
  }
  
  # Try direct connection
  my $server;
 SERVER:
  for $server (@{$CPAN::Config->{'wait_list'}}) {
    warn "CPAN::WAIT $VERSION checking $server\n" if $DEBUG;
    if ($server =~ m(^wait://([^:]+)(?::(\d+))?)) {
      ($host, $port) = ($1, $2 || 1404);
      # Constructor is inherited from Net::NNTP
      $con = WAIT::Client->new($host, Port => $port, Timeout => $TIMEOUT)
        unless $DEBUG and $DEBUG =~ /force proxy/;
      last SERVER if $con;
    }
  }
  
  # Try connection via an http proxy
  unless ($con) {
    warn "Could not connect to the WAIT server at $host port $port\n"
      unless $DEBUG and $DEBUG =~ /force proxy/;
    
    if ($CPAN::Config->{'http_proxy'}) {
      print "Trying your http proxy $CPAN::Config->{'http_proxy'}\n";
    SERVER:
      for $server (@{$CPAN::Config->{'wait_list'}}) {
        if ($server =~ m(^wait://([^:]+)(?::(\d+))?)) {
          ($host, $port) = ($1, $2 || 1404);
          $con = WAIT::Client::HTTP->new($host,
                                         Port  => $port,
                                         Proxy => $CPAN::Config->{'http_proxy'},
                                         Timeout => $TIMEOUT);
          last SERVER if $con;
        }
      }
      warn "No luck with your proxy either. Giving up\n"
        unless $con;
    } else {
      warn "You did not tell the CPAN module about an http proxy.\n" .
        "I could use such a beast instead of a direct connection.\n";
    }
  }
  
  # We had no luck.
  warn "No searching available!\n" unless $con;
  
  return $con;
}

my $con;
# Temporary file for retrieved documents
my $tmp = MM->catfile($CPAN::Config->{'cpan_home'}, 'w4c.pod');

# run a search
sub wq {
  my $self = shift;
  my $result;
  local ($") = ' ';

  $con ||= _open_connection || return;
  print "Searching for '@_'\n";
  unless ($result = $con->search(@_)) {
    print "Your query contains a syntax error.\n";
    $self->wh('wq');
  } else {
    print $con->message;
    print @{$result};
    print "Type 'wr <number>' or 'wd <number>' to examine the results\n";
  }
  $result;
}

# display hit record
sub wr {
  my $self = shift;
  my $hit  = shift;
  my $result;

  if (@_ or !$hit) {
    print "USAGE: wr <hit-number>\n";
  } else {
    $con ||= _open_connection || return;
    print "fetching info on hit number $hit\n";
    $result = $con->info($hit);
    print @$result;
  }
  $result;
}

# display hit document
sub wd {
  my $self = shift;
  my $hit  = shift;
  my $result;

  if (@_ or !$hit) {
    print "USAGE: wd <hit-number>\n";
    return;
  } 
  $con ||= _open_connection || return;
  print "Get hit number $hit ...";
  my $text  = $con->get($hit);
  my $lines = ($text)?@$text:'no';
  print " done\nGot $lines lines\nRunning perldoc on it ...\n";
  
  # perldoc does not read STDIN; so we need a temp file
  {
    my $fh = new FileHandle ">$tmp";
    $fh->print(@{$text});
  }

  # is system available every were ??
  system $^X, '-S', 'perldoc', $tmp
    and warn "Could not run '$^X -S perldoc $tmp': $?\n"
      and system $Config{'pager'}, $tmp
        and warn "Could not run '$Config{'pager'} $tmp': $?\n"
          and print @$text;
  $text;
}

sub wl {
  my $self = shift;
  my $hits = shift;
  
  if (@_) {
    print "USAGE: wl <maximum-hit-count>\n";
    return;
  }
  $con ||= _open_connection || return;
  print "Setting maximum hit count to $hits\n";
  $con->hits($hits);
}

my %HELP =
  (
   'h' => q{
'wh'           displays a short summary of commands available via the WAIT
               plugin.
'wh <command>' displays information about a the command given as argument
   },
   'q' => q{
Here are some query examples:

information retrieval               free text query 
information or retrieval            same as above 
des=information retrieval           `information' must be in the description 
des=(information retrieval)         one of them in description 
des=(information or retrieval)      same as above 
des=(information and retrieval)     both of them in description 
des=(information not retrieval)     `information' in description and
                                    `retrieval' not in description 
des=(information system*)           wild-card search
au=ilia                             author names may be misspelled

You can build arbitary boolean combination of the above examples.
The following fields are known: 

  'synopsis', 'name', 'bugs', 'author', 'example', 'description',
  'environment'

Field names may be abbreviated.
},
   'r' => q{
'wr <hit-number>'  displays the record of the selected hit. Records look
                   like this:

   source          authors/id/CHIPS/perl5.003_24.tar.gz
   headline        perl - Practical Extraction and Report Language 
   size            12786
   docid           data/perl/pod/perl.pod

'source'   is the patch relative to http://www.perl.org/CPAN/.
'headline' is the contents of the 'NAME' section of the POD document
'size'     is the size of the POD document (not the size of the tar archive)!
'docid'    is the path the POS document is stored in. It should be the
           path in the tar archive minus the version number + a 'data'
           prefix.
   },
   'l' => q{
Since answers to queries are sorted by decreasing probability of relevance,
you will probably be interested only in the first few hits. To limit the amout
of network traffic, the WAIT server only returns the best 10 hits per default.
You can change this limit with 'wl <number>'.
   },
   'd' => q{
The 'wd <hit-number>' command retrieves the POD document form the
server and stores it in the file 'w2c' in your CPAN directory. Then it
runs 'perlpod' on it. If you have problems, check if you local
'perlpod' works with absolute path names. Older versions are know to
fail. Also try to avoid fetching of large documents like 'perlfunc.pod'.
Use 'wr <hit-number>' to see how large the documents are before fetching
the actually if you have a slow connection.
},

  );

sub wh {
  my $self = shift;
  my $cmd  = shift;

  if ($cmd and $cmd =~ /(\w)$/) {
    print $HELP{$1} || "No help for 'w$1' yet.\n";
  } else {
    print qq[
Available commands:
wq        query           search the WAIT4CPAN server
wr        hit-number      display search result record
wd        hit-number      fetch the document and run perldoc on it
wl        count           limit search to <count> hits
wh        command         displays help on command if available
];
  }
  1;
}

END {
  unlink $tmp if -e $tmp;
}

1;

__DATA__

=head1 NAME

CPAN::WAIT - adds commands to search a WAIT4CPAN server to the CPAN C<shell()>

=head1 SYNOPSIS

  perl -MCPAN -e shell
  > wq au=wall
  > wr 3
  > wd 3
  > wl 20
  > wh
  > wh wq

=head1 DESCRIPTION

B<CPAN::WAIT> adds some comands to the CPAN C<shell()> to perform
searches on a WAIT server. It connects to a WAIT server using a simple
protocoll resembling NNTP as described in RFC977. It uses the
B<WAIT::Client> module to handle this connection. This in turn
inherits from B<Net::NNTP> from the F<libnet> package. So you need
B<Net::NNTP> to use this module.

If no direct connection to the WAIT server is possible, the modules
tries to connect via your HTTP proxy (as given by the CPAN
configuration). Be warned though that the emulation of the stateful
protocol via HTTP is slow.

The variable C<CPAN::WAIT::TIMEOUT> specifies the number of seconds to
wait for an answer from the server. The default is 20. You may want to
set it to some larger value if you have a slow connection.

The commands available are:

=over 

=item B<wh> [B<command>]

Displays a short help message if called without arguments. If you
provide the name of another command you will get more information on
this command if available. Currently only B<wq> will be explained.

=item B<wl> I<count>

Limit the number of hits returned in a search to I<count>. The limit
usually is set ot 10 of you don't set it.

=item B<wq> I<query>

Send a query to the server. 

Here are some query examples:

  information retrieval               free text query 
  information or retrieval            same as above 
  des=information retrieval           `information' must be in the description 
  des=(information retrieval)         one of them in description 
  des=(information or retrieval)      same as above 
  des=(information and retrieval)     both of them in description 
  des=(information not retrieval)     `information' in description and
                                      `retrieval' not in description 
  des=(information system*)           wild-card search
  au=ilia                             author names may be misspelled

You can build arbitary boolean combination of the above examples.
Field names may be abbreviated. For further information see
F<http://ls6-www.informatik.uni-dortmund.de/CPAN>

The result should look like this:

  wq au=wall

   1 8.039 a2p - Awk to Perl translator 
   2 8.039 s2p - Sed to Perl translator 
   3 8.039 perlipc - Perl interprocess communication (signals, fifos, pipes, safe subprocesses, sockets, and semaphores) 
   4 8.039 ExtUtils::DynaGlue - Methods for generating Perl extension files 
   5 8.039 h2xs - convert .h C header files to Perl extensions 
   6 8.039 Sys::Syslog, openlog, closelog, setlogmask, syslog - Perl interface to the UNIX syslog(3) calls 
   7 8.039 h2ph - convert .h C header files to .ph Perl header files 
   8 8.039 Shell - run shell commands transparently within perl 
   9 8.039 pl2pm - Rough tool to translate Perl4 .pl files to Perl5 .pm modules. 
  10 8.039 perlpod - plain old documentation 

=item B<wr> I<hit-number>

Display the Record of hit number I<hit-number>:

  wr 1
  
  source          authors/id/CHIPS/perl5.003_24.tar.gz
  headline        a2p - Awk to Perl translator 
  size            5643
  docid           data/perl/x2p/a2p.pod


=item B<wd> I<hit-number>

Fetches the full text from the server and runs B<perlpod> on it. Make
sure that you have B<perlpod> in your path. Also check if your
B<perlpod> version can handle absolute pathes. Some older versions
ironically do not find a document if the full patch is given on the
command line.

=back

=head1 AUTHOR

Ulrich Pfeifer E<lt>F<pfeifer@ls6.informatik.uni-dortmund.de>E<gt>

