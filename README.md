# Sysadmin Scripts and Programs

This repository contains scripts and programs I've written to make my
life as a system administrator easier.

Sharing under GPL v3.1 or later so others can use them if they want.

## ddns-ipv6
is a script to be called from your ipv4 DHCP server, to try to set up
forward and reverse IPv6 addresses in DNS for a client that has just
obtained an ipv4 address via DHCP, and an ipv6 address using the ipv6
autoconfiguration mechanism.  It won't add names and addresses for
clients using the privacy extension --- I assume that such clients do
not want to be accessible by name.

## test-disc.c
is a program for identifying fake flash.  It works by writing the
entire disc sequentially, then reading to see if what was written is
actually there.  This can take a very long time: fake flash drives
tend to slow down significantly when writing more than their real
capacity.  Chunks are 64kB, numbered from 0.

There are two separate things I've seen:
 - Flash drives that map the same chunk of flash into multiple ranges
 on the drive, so in the read phase, the chunk number read back is a
 multiple of the one expected.
 - Flash drives that leave regions unmapped, so they read all-ones or
 all-zeros, no matter what was written.
 
 
