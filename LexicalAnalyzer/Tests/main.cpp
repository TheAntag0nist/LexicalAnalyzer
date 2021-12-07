PROGRAMM; 
  integer x; 
  bool y; 
  string z; 
  z=”www”;
  
  WHILE 1 
   DO 
    x=x-1; 
    
    if x=5 then y=(valstr(z) > 15);  
		EXITWHILE; 
    else y=( valstr (z)<15) endif; 
  ENDWHILE; 
 
  FUNC valstr (string s); 
        int pos; 
        pos = 0;
		
        WHILE s[pos] != '\0' DO 
             pos = pos + 1; 
        ENDWHILE; 
        
		return pos; 
	ENDF; 
END.