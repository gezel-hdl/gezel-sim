        .global getcyclecount
        .type   getcyclecount,function
getcyclecount:
        swi     0x200	
	mov	pc,lr

        .global getsystemcyclecount
        .type   getsystemcyclecount,function
getsystemcyclecount:
        swi     0x201
	mov	pc,lr
