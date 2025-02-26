# EXTRACT\_FAT\_AS\_HEX(1)

## NAME
extract\_fat\_as\_hex - Extracts FAT from disk image in hex format

## SYNOPSIS
extract\_fat\_as\_hex

## DESCRIPTION
Extracts the first FAT table from a FAT32 image and converts to hex
representation (one entry per line).

## EXAMPLES
`extract_fat_as_hex bootfs.img > fat_table.hex`
