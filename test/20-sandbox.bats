#!/usr/bin/env bats

@test "sandbox: read file" {
    trep --stream-with-match=null --stream-without-match=null --file=/etc/hosts </etc/hosts
}

@test "sandbox: large input" {
    (dd if=/dev/zero count=131070 2>/dev/null; echo x) | \
          trep --stream-with-match=null --stream-without-match=null x
}

@test "sandbox: recursive search" {
    trep --recursive --stream-with-match=null --stream-without-match=null x .
}
