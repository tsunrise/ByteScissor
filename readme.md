# Byte Scissor

Byte scissor is tool to split a secret file into fragments.  The file can be recovered if and only if any required amount of fragments are merged. The parameter `n` (number of fragments) and `k` (fragments required to merge) can be specified. 

## Get Started

### Split a file into multiple fragments

```bash
$ byteScissor split <path> <number of fragments> <number of required fragments to merge>
```

`path`: The path (either relative or absolute) of the file.

`number of fragments`: Parameter `n`. The secret file will be split to n copies. 

`number of required fragments to merge`: Parameter `k`. The secret file can only be recovered if k of copies are merged. 



### Split a number into multiple numbers

```bash
$ byteScissor split-n <number> <number of fragments> <number of required fragments to merge>
```

`number`: The number to be split. The number should be in range \[0, 4294967295\]

`number of fragments` and `number of required fragments to merge` are the same as above. 



### Recover a file

```bash
$ byteScissor merge --output <output path> --input <input paths>...
```

One can specify the path of the merged file and a list of input paths. `--output` can be replaced with `-o` and `--input` can be replaced with `-i`. 



### Recover a number

```bash
$ byteScissor merge-n <id> <value> ...
```

`id`: The position of the value. For example, if `split-n` returns `189 172 193`, the id of `189` is 1; the id of `172` is 2. 

`value`: The values to be merged. 



### Example: Split an Image

Suppose we want to split an image `image.png` into 7 fragments. Combining any 4 fragments can recover this image. 

To split the image, use the following command: 

```bash
$ byteScissor split ./image.png 7 4
```

The following fragments will be generated: 

```
image.png.1.part
image.png.2.part
image.png.3.part
image.png.4.part
image.png.5.part
image.png.6.part
image.png.7.part
```

Suppose one has the first, fourth, fifth, and sixth fragments. To merge the file into `merged.png`, use the following command:

```bash
$ byteScissor merge -o merged.png -i ./image.png.1.part ./image.png.4.part ./image.png.5.part ./image.png.6.part
```



## Contribute

Feel free to create an issue or make a merge request. 

