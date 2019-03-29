Manipulating file names
-----------------------
1. str_2_dir_and_file

   Examples:
    "./dir/file.txt" --> "dir" & "file.txt"
    "dir/file.txt"   --> "dir" & "file.txt"
    "./dir/"         --> "dir"
    "dir"            --> "dir"

2. fileNumber

   Examples
   "file.txt"        --> "file001.txt", "file002.txt", etc.

3. numStr: int to string
   
   Examples
   87               --> "0087"

4. countStr
   e.g. "0087"      --> "0088"

String Interpretation
---------------------
1. countWord: Interpret spaces and tabs as white space
  "hello =)"         --> 2
  "   hello =)"      --> 2

2. getWord: Find first word (string between spaces/tabs) 
            in a string
  "hello =)"         --> "hello"
  "   hello =)"      --> "hello"


