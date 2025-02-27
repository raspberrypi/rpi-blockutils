# APPLY\_SIMG(1)

## NAME
apply\_simg - Apply a sparse image to an existing file

## SYNOPSIS
apply\_simg

## DESCRIPTION
Applies a sparse image to an existing file without truncating it. 'Don't care'
bytes are skipped rather than zeroed.

## EXAMPLES
`apply_simg bootfs.simg existing.img`

## SEE ALSO
`simg2img`(1)
