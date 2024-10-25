let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/Documents/coding/RoombaController
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +46 src/main.cpp
badd +1 src/Graph.cpp
badd +58 src/Graph.h
badd +1 src/Node.cpp
badd +1 src/Node.h
badd +21 src/Pillar.h
badd +31 src/Pose2D.h
badd +37 src/Pose2D.cpp
badd +24 src/Pillar.cpp
badd +20 Field.h
badd +1 src/Field.cpp
badd +0 src/Field.h
argglobal
%argdel
$argadd src/main.cpp
edit src/Graph.cpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
3wincmd h
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd w
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd w
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 35 + 36) / 72)
exe 'vert 1resize ' . ((&columns * 141 + 282) / 564)
exe '2resize ' . ((&lines * 34 + 36) / 72)
exe 'vert 2resize ' . ((&columns * 141 + 282) / 564)
exe '3resize ' . ((&lines * 35 + 36) / 72)
exe 'vert 3resize ' . ((&columns * 141 + 282) / 564)
exe '4resize ' . ((&lines * 34 + 36) / 72)
exe 'vert 4resize ' . ((&columns * 141 + 282) / 564)
exe 'vert 5resize ' . ((&columns * 139 + 282) / 564)
exe 'vert 6resize ' . ((&columns * 140 + 282) / 564)
argglobal
balt src/Pillar.cpp
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 42 - ((34 * winheight(0) + 17) / 35)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 42
normal! 0
wincmd w
argglobal
if bufexists(fnamemodify("src/Pillar.cpp", ":p")) | buffer src/Pillar.cpp | else | edit src/Pillar.cpp | endif
if &buftype ==# 'terminal'
  silent file src/Pillar.cpp
endif
balt src/Pose2D.cpp
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 24 - ((23 * winheight(0) + 17) / 34)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 24
normal! 0
wincmd w
argglobal
if bufexists(fnamemodify("src/Field.h", ":p")) | buffer src/Field.h | else | edit src/Field.h | endif
if &buftype ==# 'terminal'
  silent file src/Field.h
endif
balt src/Field.cpp
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 16 - ((15 * winheight(0) + 17) / 35)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 16
normal! 013|
wincmd w
argglobal
if bufexists(fnamemodify("src/Field.cpp", ":p")) | buffer src/Field.cpp | else | edit src/Field.cpp | endif
if &buftype ==# 'terminal'
  silent file src/Field.cpp
endif
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 8 - ((7 * winheight(0) + 17) / 34)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 8
normal! 02|
wincmd w
argglobal
if bufexists(fnamemodify("src/main.cpp", ":p")) | buffer src/main.cpp | else | edit src/main.cpp | endif
if &buftype ==# 'terminal'
  silent file src/main.cpp
endif
balt src/Pose2D.cpp
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 30 - ((29 * winheight(0) + 35) / 70)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 30
normal! 0
wincmd w
argglobal
if bufexists(fnamemodify("src/Graph.h", ":p")) | buffer src/Graph.h | else | edit src/Graph.h | endif
if &buftype ==# 'terminal'
  silent file src/Graph.h
endif
balt src/Pose2D.h
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=0
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
let &fdl = &fdl
let s:l = 52 - ((42 * winheight(0) + 35) / 70)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 52
normal! 0
wincmd w
4wincmd w
exe '1resize ' . ((&lines * 35 + 36) / 72)
exe 'vert 1resize ' . ((&columns * 141 + 282) / 564)
exe '2resize ' . ((&lines * 34 + 36) / 72)
exe 'vert 2resize ' . ((&columns * 141 + 282) / 564)
exe '3resize ' . ((&lines * 35 + 36) / 72)
exe 'vert 3resize ' . ((&columns * 141 + 282) / 564)
exe '4resize ' . ((&lines * 34 + 36) / 72)
exe 'vert 4resize ' . ((&columns * 141 + 282) / 564)
exe 'vert 5resize ' . ((&columns * 139 + 282) / 564)
exe 'vert 6resize ' . ((&columns * 140 + 282) / 564)
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let &winminheight = s:save_winminheight
let &winminwidth = s:save_winminwidth
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
nohlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
