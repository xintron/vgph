#!/bin/sh

testHelp() {
    help=$(./vgph 2>&1)
    text="usage: ./vgph [name:]value [...]"
    assertEquals "$text" "$help"

    help=$(./vgph -h 2>&1)
    assertEquals "$text" "$help"

    help=$(./vgph --help 2>&1)
    assertEquals "$text" "$help"
}

# Here we build the result dynamically using the info we get from tput to match
# the size of the terminal that vgph will use
testArgvDataset() {
    graph="$(./vgph foo:10 bar:28.5)"
    let "cols=$(tput cols) - 6"
    res="foo []
bar ["
    for i in $(seq 1 $cols); do res="$res#"; done
    res="$res]"
    assertEquals "$res" "$graph"
}

. shunit2
