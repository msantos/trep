#!/usr/bin/env bats

stdin1() {
    cat << EOF
test
123
EOF
}

export -f stdin1

@test "stream: stdout" {
    run bash -c "stdin1 | trep --stream-with-match=stdout --stream-without-match=null test > /dev/null"
    echo "$output"
    [ -z "$output" ]
}

@test "stream: stderr" {
    run bash -c "stdin1 | trep --stream-with-match=stderr --stream-without-match=null test 2> /dev/null"
    echo "$output"
    [ -z "$output" ]
}

@test "stream: match=null nomatch=null" {
    run bash -c "stdin1 | trep --stream-with-match=null --stream-without-match=null test"
    echo "$output"
    [ -z "$output" ]
}

@test "stream: match=stdout nomatch=null" {
    run bash -c "stdin1 | trep --stream-with-match=stdout --stream-without-match=null test"
    [ "$output" == "test" ]
}

@test "stream: match=null nomatch=stdout" {
    run bash -c "stdin1 | trep --stream-with-match=null --stream-without-match=stdout test"
    [ "$output" == "123" ]
}

@test "stream: match=stdout nomatch=stdout" {
    run bash -c "stdin1 | trep --stream-with-match=stdout --stream-without-match=stdout test"
    [ "${lines[0]}" == "test" ]
    [ "${lines[1]}" == "123" ]
}

@test "stream: match=stderr nomatch=null" {
    run bash -c "stdin1 | trep --stream-with-match=stderr --stream-without-match=null test"
    [ "$output" == "test" ]
}

@test "stream: match=null nomatch=stderr" {
    run bash -c "stdin1 | trep --stream-with-match=null --stream-without-match=stderr test"
    [ "$output" == "123" ]
}

@test "stream: match=stderr nomatch=stderr" {
    run bash -c "stdin1 | trep --stream-with-match=stderr --stream-without-match=stderr test"
    [ "${lines[0]}" == "test" ]
    [ "${lines[1]}" == "123" ]
}

@test "stream: match=stdout nomatch=stderr" {
    BATS_TEST_TMPDIR="${BATS_TEST_TMPDIR-$(mktemp -d)}"
    stdin1 | trep --stream-with-match=stdout --stream-without-match=stderr test > "$BATS_TEST_TMPDIR/stdout" 2> "$BATS_TEST_TMPDIR/stderr"
    run cat "$BATS_TEST_TMPDIR/stdout"
    [ "$output" == "test" ]
    run cat "$BATS_TEST_TMPDIR/stderr"
    [ "$output" == "123" ]
}

@test "stream: match=stderr nomatch=stdout" {
    run bash -c "stdin1 | trep --stream-with-match=stderr --stream-without-match=stdout test"
    [ "${lines[0]}" == "test" ]
    [ "${lines[1]}" == "123" ]
}

@test "stream: prepend label: match=stderr nomatch=stdout" {
    run bash -c "stdin1 | trep --label=foo -H --stream-with-match=stderr --stream-without-match=stdout test"
    [ "${lines[0]}" == "foo:test" ]
    [ "${lines[1]}" == "foo:123" ]
}
