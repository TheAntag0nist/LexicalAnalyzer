FUNC valstr (string s); 
FUNC valstr_1 (string s, bool info);

PROGRAMM; 
  integer x;
  bool y; 
  bool whileControl;
  string z; 
  z="www";
  whileControl = 1;
  
  WHILE whileControl == 1 
   DO 
    x=x-1; 
    
    if x==5 then y=valstr(z) > 15;  
		EXITWHILE; 
    else y=valstr(z) <15 endif; 
  ENDWHILE; 
END.

FUNC valstr (string s); 
    integer pos; 
    pos = 0;
		
    WHILE s[pos] != '\0' DO 
        pos = pos + 1; 
    ENDWHILE; 
        
	return pos; 
ENDF;