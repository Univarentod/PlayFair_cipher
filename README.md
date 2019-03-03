# PlayFair_cipher
Based on the PlayFair cipher's description on the wiki

The rules adopted:
  
  To generate the keytable:
    
    The 'J' will be omitted.
  
  Encryption:
    At the beginning, divide the input into pairs of characters.
    
    1. If both letters are the same (or only one letter is left), add an 'X' after the first letter. If both letters 
      are 'X', add an 'Q' after the first letter.
    
    2. If the letters appear on the same row of your table, replace them with the letters to their immediate right
        respectively (wrapping around to the left side of the row if a letter in the original pair was on the right 
        side of the row).
    
    3. If the letters appear on the same column of your table, replace them with the letters immediately below 
      respectively (wrapping around to the top side of the column if a letter in the original pair was on the bottom
      side of the column).
    
    4. If the letters are not on the same row or column, replace them with the letters on the same row respectively 
      but at the other pair of corners of the rectangle defined by the original pair.

  Decryption:
    
    1. If an 'X' is located between two same characters or at the end of the plaintext, drop the 'X'. If an 'Q' is is
      located between two 'X' or at the end of the plaintext, drop the 'Q'.
    
    2, 3, 4. Use the inverse (opposite) of the last 3 rules
