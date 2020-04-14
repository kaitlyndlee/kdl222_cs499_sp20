Command line used to find this crash:

/usr/local/bin/afl-fuzz -o afl_out -m 500 -i afl_in/ -t 50 -- /home/user/kdl222_cs499_sp20/assignment3/symexwins_alf --input_test_file @@ --abort_on_fail --no_fork

If you can't reproduce a bug outside of afl-fuzz, be sure to set the same
memory limit. The limit used for this fuzzing session was 500 MB.

Need a tool to minimize test cases before investigating the crashes or sending
them to a vendor? Check out the afl-tmin that comes with the fuzzer!

Found any cool bugs in open-source tools using afl-fuzz? If yes, please drop
me a mail at <lcamtuf@coredump.cx> once the issues are fixed - I'd love to
add your finds to the gallery at:

  http://lcamtuf.coredump.cx/afl/

Thanks :-)
