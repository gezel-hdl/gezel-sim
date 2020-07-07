AC_DEFUN([AX_ARG_DISABLE],
    [AC_ARG_ENABLE([$1],[$2],
m4_ifnblank([$3], [
$3]), [enable_$1=yes]m4_ifnblank(
[$4],[
$4]))])
