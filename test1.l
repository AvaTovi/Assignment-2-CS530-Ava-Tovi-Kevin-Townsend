0000    TEST1   START   2000              
2000    BEGIN   LDA     COUNT     03201F  
2006            STA     COUNT     0F2019  
2009            BASE    DATA              
2010    LOOP    LDA     DATA,X    03A015  
2013            ADD     VALUE     1B200F  
2016            STA     DATA,X    0FA00F  
2019            TIX     COUNT     2F2006  
201C            JLT     LOOP      3B2FF1  
201F            RSUB              4F0000  
2022    COUNT   WORD    03        000003  
2025    VALUE   WORD    01        000001  
2028    DATA    RESW    10                
2046            END     BEGIN             
