# File Splitter

This program implements the classic secret sharing scheme. It splits the secret file into $$n$$ copies. The file can be recovered if and only if any $$k$$ of the copies are merged. The parameter $$n$$ and $$k$$ can be specified. 

## Get Started

### Split a file into several copies

```bash
$ splitter split <path> <nCopies> <nRequired>
```

`path`: The path (either relative or absolute) of the file.

`nCopies`: Parameter $$n$$. The secret file will be split to n copies. 

`nRequired`: Parameter $$k$$. The secret file can only be recovered if k of copies are merged. 



### Split a number into several numbers

```bash
$ splitter program split-n <number> <nCopies> <nRequired>
```

`number`: The number to be split. 

`nCopies` and `nRequired` are the same as above. 



### Recover a file

🚧 Under development. Coming soon. 



### Recover a number

```bash
$ splitter merge-n <id> <value> ...
```

`id`: The position of the value. For example, if `split-n` returns `189 172 193`, the id of `189` is 1; the id of `172` is 2. 

`value`: The values to be merged. 



## Contribute

Because this is my first C++ project, I believe there should be a ton of things to be fixed. Feel free to contact me, create an issue, or make a merge request. 

