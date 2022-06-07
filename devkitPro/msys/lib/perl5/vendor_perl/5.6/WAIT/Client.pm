#!/usr/bin/perl
#                              -*- Mode: Perl -*- 
# $Basename: Client.pm $
# $Revision: 1.3 $
# Author          : Ulrich Pfeifer
# Created On      : Fri Jan 31 10:49:37 1997
# Last Modified By: Ulrich Pfeifer
# Last Modified On: Mon Aug 11 17:06:51 1997
# Language        : CPerl
# Update Count    : 88
# Status          : Unknown, Use with caution!
# 
# (C) Copyright 1997, Ulrich Pfeifer, all rights reserved.
# 
# 

package WAIT::Client;
use Net::NNTP ();
use Net::Cmd qw(CMD_OK);
use Carp;
use strict;
use vars qw(@ISA);

@ISA = qw(Net::NNTP);

sub search
{
  my $wait = shift;
  
  $wait->_SEARCH(@_)
    ? $wait->read_until_dot()
      : undef;
}

sub info
{
  @_ == 2 or croak 'usage: $wait->info( HIT-NUMBER )';
  my $wait = shift;
  
  $wait->_INFO(@_)
    ? $wait->read_until_dot()
      : undef;
}

sub get
{
  @_ == 2 or croak 'usage: $wait->info( HIT-NUMBER )';
  my $wait = shift;
  
  $wait->_GET(@_)
    ? $wait->read_until_dot()
      : undef;
}

sub database
{
  @_ == 2 or croak 'usage: $wait->database( DBNAME )';
  my $wait = shift;
  
  $wait->_DATABASE(@_);
}

sub table
{
  @_ == 2 or croak 'usage: $wait->table( TABLE )';
  my $wait = shift;
  
  $wait->_TABLE(@_);
}

sub hits
{
  @_ == 2 or croak 'usage: $wait->hits( NUM-MAX-HITS )';
  my $wait = shift;
  
  $wait->_HITS(@_);
}

sub _SEARCH   { shift->command('SEARCH',   @_)->response == CMD_OK }
sub _INFO     { shift->command('INFO',     @_)->response == CMD_OK }
sub _GET      { shift->command('GET',      @_)->response == CMD_OK }
sub _DATABASE { shift->command('DATABASE', @_)->response == CMD_OK }
sub _TABLE    { shift->command('TABLE',    @_)->response == CMD_OK }
sub _HITS     { shift->command('HITS',     @_)->response == CMD_OK }

# The following is a real hack. Don't look at it ;-) It tries to
# emulate a stateful protocol over HTTP which is weird and slow.
package WAIT::Client::HTTP;
use Net::Cmd;
use vars qw(@ISA);
use Carp;

@ISA = qw(WAIT::Client);

sub new {
  my $type = shift;
  my $host = shift;
  my %parm = @_;
  my ($proxy, $port) = ($parm{Proxy} =~ m{^(?:http://)(\S+)(?::(\d+))});
  $port = 80 unless $port;
  
  my $self = {
              proxy_host => $proxy,
              proxy_port => $port,
              wais_host  => $host,
              wais_port  => $parm{Port},
              timeout    => $parm{Timeout}||120,
             };
  bless $self, $type;

  my $con;
  if ($con = $self->command('HELP') and $con->response == CMD_INFO) {
    return $self;
  } else {
    return;
  }
}

sub command {
  my $self = shift;
  my $con  =                    # Constructor inherited from IO::Socket::INET
    WAIT::Client::HTTP::Handle->new
      (
       PeerAddr => $self->{proxy_host},
       PeerPort => $self->{proxy_port},
       Proto    => 'tcp',
      );
  return unless $con;

  $con->timeout($self->{timeout}) if defined $self->{timeout};
  my $cmd = join ' ', @_;
  
  if ($self->{hits}) {
    $cmd = "HITS $self->{hits}:$cmd";
  }
  $cmd = "Command: $cmd";
  $con->autoflush(1);
  
  $con->printf("POST http://$self->{wais_host}:$self->{wais_port} ".
               "HTTP/1.0\nContent-Length: %d\n\n$cmd",
               length($cmd));
  
  unless ($con->response == CMD_OK) {
    warn "No greeting from server\n";
  }
  if ($self->{hits}) {
    unless ($con->response == CMD_OK) {
      warn "Hits not aknowledged\n";
    }
  }
  $self->{con} = $con;
  $con;
}

# We map here raw document id's to rank numbers and back for
# convenience. Besides that the following search(), info(), and get()
# are obsolete.

sub search
{
  my $wait = shift;
  
  if ($wait->_SEARCH(@_)) {
    my $r = $wait->read_until_dot();
    my $i = 1;
    
    delete $wait->{'map'};
    for (@$r) {
      if (s/^(\d+)/sprintf("%4d",$i)/e) {
        $wait->{'map'}->[$i++] = $1;
      } 
    }
    return $r;
  }
  return undef;
}

sub info
{
  @_ == 2 or croak 'usage: $wait->info( HIT-NUMBER )';
  my $wait = shift;
  my $num  = shift;

  unless ($wait->{'map'}->[$num]) {
    print "No such hit: $num\n";
    return;
  }
  $wait->_INFO($wait->{'map'}->[$num])
    ? $wait->read_until_dot()
      : undef;
}

sub get
{
  @_ == 2 or croak 'usage: $wait->info( HIT-NUMBER )';
  my $wait = shift;
  my $num  = shift;

  unless ($wait->{'map'}->[$num]) {
    print "No such hit: $num\n";
    return;
  }
  $wait->_GET($wait->{'map'}->[$num])
    ? $wait->read_until_dot()
      : undef;
}

# We must store the hit count locally
sub _HITS {
  my $self = shift;
  my $hits = shift;

  $self->{hits} = $hits;
  ["Setting maximum hit count to $hits"];
}

# We should use AUTOLOAD here. I know ;-)
sub read_until_dot {shift->{con}->read_until_dot(@_)}
sub message        {shift->{con}->message(@_)}

package WAIT::Client::HTTP::Handle;
use vars qw(@ISA);

@ISA = qw(Net::Cmd IO::Socket::INET);


1;
