# myshell
myshell with additional coreutils

description will evolve soon

[<h2>myshell</h2>](myshell)
<p>
  Similar to windows or ubuntu <b>terminal</b><br />
  Supports <b>built-in</b> commands and execution of <b>external</b> programs<br />
  
  <ul>
  Built-in commands:
  <li><b>merrno [-h|--help]</b> displays exit code of the last command (if no commands were executed, prints 0)</li>
  <li><b>mpwd [-h|--help]</b> outputs name of the current directory</li>
  <li><b>mcd &lt;path&gt; [-h|--help]</b> navigates to the given directory</li>
  <li><b>mexit &lt;exit code&gt; [-h|--help]</b> exits with given exit code (if no exit code is provided, exits with 0 code)</li>
  </ul>
  
  Every other command is considered external and is executed using fork-exec
</p>

[<h2>myls</h2>](myls)
<p>
  Prints information about files or directories<br />
  Similar to <b>ls</b> coreutil in ubuntu <br /><br />
  <b>Usage:</b><br />
  &nbsp;&nbsp;&nbsp;&nbsp;<b>myls [path] [-l] [-h|--help] [--sort=U|S|t|X|D|s] [-r] [-R]</b>
  
  <ul>
    <li>
      <b>path</b> relative or absolute path to file
    </li>
    <li>
      <b>-l</b> print additional information about the file (<i>size</i>, <i>time of last modification</i>)
    </li>
    <li>
      <b>--sort</b> defines sorting criteria files will be sorted by
      <ul>
        <li><b>U</b> unsorted</li>
        <li><b>S</b> size</li>
        <li><b>t</b> time of last modification</li>
        <li><b>X</b> extension</li>
        <li><b>N</b> name (<i>default</i>)</li>
      </ul>
      With each of these, you can use 2 more identifiers:
      <ul>
        <li><b>D</b> directories first</li>
        <li><b>s</b> special files are printed separately</li>
      </ul>
    </li>
    <li>
      <b>-r</b> files are reversed with respect to sorting criteria
    </li>
    <li>
      <b>-F</b> prints types of special files:
      <ul>
        <li><b>*</b> executable</li>
        <li><b>@</b> symlink</li>
        <li><b>|</b> named pipe</li>
        <li><b>=</b> socket</li>
        <li><b>?</b> all the rest</li>
      </ul>
    </li>
    <li>
      <b>-R</b> recursive traversal inside directories
    </li>
    
  </ul>
  
  <b>Example:</b><br />
  <i>input:</i><br />
  &nbsp;&nbsp;&nbsp;&nbsp;./myls --sort=Ns -F -r<br />
  <i>output:</i><br />
  &nbsp;&nbsp;&nbsp;&nbsp;*myls<br />
  &nbsp;&nbsp;&nbsp;&nbsp;?Makefile /kokoko /CMakeFiles ?CMakeCache.txt ?cmake_install.cmake<br />
  
  If there are <b>invalid options</b>, exits with code <b>1</b> <br />
  If there are files that <b>does not exist</b>, writes a warning to cerr and continues executing. Exit code is <b>1</b>
</p>
