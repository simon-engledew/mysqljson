# mysqljson

Command-line tool to canonicalize JSON the same way MySQL 5.7 does, built against the MySQL internals.

```
echo '[14970.911769838869, 20000000200000000000, -0, 1.0, "hello & here", {"key1": 2, "key1": 1}]' | mysqljson
[14970.911769838867, 2.00000002e19, 0, 1, "hello & here", {"key1": 2}]
```

Disclaimer: I am not a C++ programmer, so there is a strong chance this thing leaks resources everywhere!
