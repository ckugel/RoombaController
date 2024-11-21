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
badd +502 src/main.cpp
badd +1 src/Field.cpp
badd +1 src/Graph.cpp
badd +1 src/Node.cpp
badd +1 src/Pillar.cpp
badd +1 src/Pose2D.cpp
badd +1 src/Field.hpp
badd +1 src/Graph.hpp
badd +1 src/Node.hpp
badd +1 src/Pillar.hpp
badd +1 src/Pose2D.hpp
badd +1 src/Hole.hpp
badd +89 src/Hole.cpp
badd +80 src/HoleManager.cpp
badd +37 src/HoleManager.hpp
badd +1 src/SlopeIntercept.hpp
badd +1 src/SlopeIntercept.cpp
badd +0 src/util.h
badd +0 src/util.c
argglobal
%argdel
$argadd src/Field.cpp
$argadd src/Graph.cpp
$argadd src/Node.cpp
$argadd src/Pillar.cpp
$argadd src/Pose2D.cpp
$argadd src/main.cpp
set stal=2
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabnew +setlocal\ bufhidden=wipe
tabrewind
edit src/Field.hpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
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
exe '1resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 1resize ' . ((&columns * 107 + 70) / 140)
exe '2resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 2resize ' . ((&columns * 32 + 70) / 140)
argglobal
if bufexists(fnamemodify("src/Field.hpp", ":p")) | buffer src/Field.hpp | else | edit src/Field.hpp | endif
if &buftype ==# 'terminal'
  silent file src/Field.hpp
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
let s:l = 15 - ((7 * winheight(0) + 13) / 26)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 15
normal! 0
wincmd w
argglobal
if bufexists(fnamemodify("src/Field.cpp", ":p")) | buffer src/Field.cpp | else | edit src/Field.cpp | endif
if &buftype ==# 'terminal'
  silent file src/Field.cpp
endif
balt src/Field.hpp
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
let s:l = 1 - ((0 * winheight(0) + 13) / 26)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
wincmd w
exe '1resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 1resize ' . ((&columns * 107 + 70) / 140)
exe '2resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 2resize ' . ((&columns * 32 + 70) / 140)
tabnext
edit src/Graph.hpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
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
exe '1resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 1resize ' . ((&columns * 107 + 70) / 140)
exe '2resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 2resize ' . ((&columns * 32 + 70) / 140)
argglobal
2argu
if bufexists(fnamemodify("src/Graph.hpp", ":p")) | buffer src/Graph.hpp | else | edit src/Graph.hpp | endif
if &buftype ==# 'terminal'
  silent file src/Graph.hpp
endif
balt src/Graph.cpp
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
let s:l = 1 - ((0 * winheight(0) + 13) / 26)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
wincmd w
argglobal
2argu
balt src/Graph.hpp
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
let s:l = 1 - ((0 * winheight(0) + 13) / 26)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
wincmd w
exe '1resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 1resize ' . ((&columns * 107 + 70) / 140)
exe '2resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 2resize ' . ((&columns * 32 + 70) / 140)
tabnext
edit src/Node.hpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
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
exe '1resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 1resize ' . ((&columns * 107 + 70) / 140)
exe '2resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 2resize ' . ((&columns * 32 + 70) / 140)
argglobal
3argu
if bufexists(fnamemodify("src/Node.hpp", ":p")) | buffer src/Node.hpp | else | edit src/Node.hpp | endif
if &buftype ==# 'terminal'
  silent file src/Node.hpp
endif
balt src/Node.cpp
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
let s:l = 1 - ((0 * winheight(0) + 13) / 26)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
wincmd w
argglobal
3argu
balt src/Node.hpp
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
let s:l = 1 - ((0 * winheight(0) + 13) / 26)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 02|
wincmd w
exe '1resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 1resize ' . ((&columns * 107 + 70) / 140)
exe '2resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 2resize ' . ((&columns * 32 + 70) / 140)
tabnext
edit src/Pillar.hpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
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
exe '1resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 1resize ' . ((&columns * 107 + 70) / 140)
exe '2resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 2resize ' . ((&columns * 32 + 70) / 140)
argglobal
4argu
if bufexists(fnamemodify("src/Pillar.hpp", ":p")) | buffer src/Pillar.hpp | else | edit src/Pillar.hpp | endif
if &buftype ==# 'terminal'
  silent file src/Pillar.hpp
endif
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
let s:l = 1 - ((0 * winheight(0) + 13) / 26)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
wincmd w
argglobal
4argu
balt src/Pillar.hpp
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
let s:l = 1 - ((0 * winheight(0) + 13) / 26)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
wincmd w
exe '1resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 1resize ' . ((&columns * 107 + 70) / 140)
exe '2resize ' . ((&lines * 26 + 21) / 42)
exe 'vert 2resize ' . ((&columns * 32 + 70) / 140)
tabnext
edit src/Pose2D.hpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
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
exe 'vert 1resize ' . ((&columns * 52 + 70) / 140)
exe 'vert 2resize ' . ((&columns * 87 + 70) / 140)
argglobal
5argu
if bufexists(fnamemodify("src/Pose2D.hpp", ":p")) | buffer src/Pose2D.hpp | else | edit src/Pose2D.hpp | endif
if &buftype ==# 'terminal'
  silent file src/Pose2D.hpp
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
let s:l = 45 - ((22 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 45
normal! 01|
wincmd w
argglobal
5argu
balt src/Pose2D.hpp
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
let s:l = 1 - ((0 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
wincmd w
exe 'vert 1resize ' . ((&columns * 52 + 70) / 140)
exe 'vert 2resize ' . ((&columns * 87 + 70) / 140)
tabnext
edit src/HoleManager.hpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
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
exe 'vert 1resize ' . ((&columns * 48 + 70) / 140)
exe 'vert 2resize ' . ((&columns * 91 + 70) / 140)
argglobal
1argu
if bufexists(fnamemodify("src/HoleManager.hpp", ":p")) | buffer src/HoleManager.hpp | else | edit src/HoleManager.hpp | endif
if &buftype ==# 'terminal'
  silent file src/HoleManager.hpp
endif
balt src/HoleManager.cpp
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
let s:l = 37 - ((15 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 37
normal! 011|
wincmd w
argglobal
1argu
if bufexists(fnamemodify("src/HoleManager.cpp", ":p")) | buffer src/HoleManager.cpp | else | edit src/HoleManager.cpp | endif
if &buftype ==# 'terminal'
  silent file src/HoleManager.cpp
endif
balt src/HoleManager.hpp
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
let s:l = 80 - ((29 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 80
normal! 0
wincmd w
exe 'vert 1resize ' . ((&columns * 48 + 70) / 140)
exe 'vert 2resize ' . ((&columns * 91 + 70) / 140)
tabnext
edit src/util.c
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
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
exe 'vert 1resize ' . ((&columns * 70 + 70) / 140)
exe 'vert 2resize ' . ((&columns * 69 + 70) / 140)
argglobal
if bufexists(fnamemodify("src/util.c", ":p")) | buffer src/util.c | else | edit src/util.c | endif
if &buftype ==# 'terminal'
  silent file src/util.c
endif
balt src/util.h
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
let s:l = 1 - ((0 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
wincmd w
argglobal
if bufexists(fnamemodify("src/util.h", ":p")) | buffer src/util.h | else | edit src/util.h | endif
if &buftype ==# 'terminal'
  silent file src/util.h
endif
balt src/util.c
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
let s:l = 1 - ((0 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
wincmd w
exe 'vert 1resize ' . ((&columns * 70 + 70) / 140)
exe 'vert 2resize ' . ((&columns * 69 + 70) / 140)
tabnext
edit src/Hole.hpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
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
exe 'vert 1resize ' . ((&columns * 46 + 70) / 140)
exe 'vert 2resize ' . ((&columns * 93 + 70) / 140)
argglobal
if bufexists(fnamemodify("src/Hole.hpp", ":p")) | buffer src/Hole.hpp | else | edit src/Hole.hpp | endif
if &buftype ==# 'terminal'
  silent file src/Hole.hpp
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
let s:l = 26 - ((17 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 26
normal! 011|
wincmd w
argglobal
if bufexists(fnamemodify("src/Hole.cpp", ":p")) | buffer src/Hole.cpp | else | edit src/Hole.cpp | endif
if &buftype ==# 'terminal'
  silent file src/Hole.cpp
endif
balt src/Hole.hpp
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
let s:l = 89 - ((24 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 89
normal! 013|
wincmd w
exe 'vert 1resize ' . ((&columns * 46 + 70) / 140)
exe 'vert 2resize ' . ((&columns * 93 + 70) / 140)
tabnext
edit src/main.cpp
argglobal
6argu
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
let s:l = 502 - ((12 * winheight(0) + 19) / 39)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 502
normal! 04|
tabnext 7
set stal=1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
