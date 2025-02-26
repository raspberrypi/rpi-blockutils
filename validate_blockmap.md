# VALIDATE\_BLOCKMAP(1)

## NAME
validate\_blockmap - Validates blockmaps

## SYNOPSIS
validate\_blockmap

## DESCRIPTION
Reads an image file block by block and raises errors if blocks marked unused by
the blockmap are found to contain data.

## EXAMPLES
`validate_blockmap <BLOCK_SIZE> <BLOCK_MAP> <IMG>`
