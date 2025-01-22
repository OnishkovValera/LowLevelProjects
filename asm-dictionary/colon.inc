%define last 0

%macro colon 2
    %if %0 != 2
        %error "input format: colon "character", label"
    %endif
    
    %ifidn %1, ""
        %error "first argument missing"
    %endif
    
    %ifidn %2, ""
        %error "second argument missing"
    %endif

	%ifnid %2
    	%error "The second argument must be a label"
	%endif

    %2:
        dq last
        db %1, 0
    %define last %2
%endmacro
