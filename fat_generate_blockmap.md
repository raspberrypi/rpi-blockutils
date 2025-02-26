# FAT\_GENERATE\_BLOCKMAP(1)

## NAME
fat\_generate\_blockmap - Creates blockmaps for FAT32 disk images

## SYNOPSIS
fat\_generate\_blockmap

## DESCRIPTION
Generates a blockmap for a FAT32 disk image by searching for unused clusters.

## EXAMPLES
`fat_generate_blockmap bootfs.img > bootfs.blockmap`

## SEE ALSO
`blockmap_img2simg`(1)
`fatcat`(1)
`extract_fat_as_hex`(1)
`ascii_fat_to_blockmap`(1)
