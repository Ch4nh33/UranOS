# UranOS
- Develop Operating System for x86 Architecture
- But, this OS is Based on Kawai Hidemi's "HariboteOS" then Customizing in this OS
- So You may also see a little bit of Japanese
  
# Operating System based on x86
- 32bit to train the concept of Operating System
    - Portable microkernel-based multiserver OS designed from Assembly and C language
- The UranOS developed by Chan hee Yi (Republic of Korea)
- You can learn how I develop the OS on here
    - If you want reading in English, you must translate Korean(and little bit of Japanese) to English
- Each folder's name is some kind of version of the OS.
- So the more higher, the better OS implemented.
- Here is the list of each folder and some description about them.


# UranOS Screen
![git_readme_img001](https://github.com/Ch4nh33/UranOS/assets/78791504/ffca717c-5069-45b3-bfc1-f0203c31aea5)

# Command line 
### dir
- Showing all directory
- In Linux, instead ls -al command

### mem
- Displays the current memory state in OS

### cls
- clear screen
- In Linux, instead **clear** command
### start
- start api modules
- In Linux, same of **execute directory**
### exit
- exit process
- In Linux, same of **ctrl+c or quit** command

## API List in UranOS

### gview
- Can see the Graphic file → png, jpg, bmp, etc..<br>
<pre>
<code>
gview (file name.extention name(png, jpg...)
</code>
</pre>

### tview
- Can see the text file
<pre>
<code>
tview (file name.txt) -w(weight) -h(height)
</code>
</pre>

### bball
- That means beautiful ball
- Drawing a ball with a line

### Invader
- You can play the Galag game
- Arrow to move, Attack is space key

### mmlplay.daigo.mml
- Mml player in UranOS

### Lines
- Showing lines in new  window

### Sosu
- It represents prime numbers from 1 to 100

## The Other API is just test API 
- If you loading The other API, You can start command console
<pre>
<code>
start (API names...)
</code>
</pre>

# License
- UranOS is open source, free software
- Its source code is available under the BSD
- license Some third party components are licensed under the GPL
