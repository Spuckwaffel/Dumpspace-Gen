<h1 align="left">Dumpspace Generator<br /><img src="logo.png?raw=true" alt="Logo" height="250px" align="right" /></h1>
<p align="left"><img src="https://img.shields.io/github/license/Spuckwaffel/Dumpspace-Gen" alt="License" /></p>
<p>The Dumpspace Gen library adds support for your dumper for generating Dumspace files of your game.</p>
<p><a href="#features">Features</a> &bull; <a href="#getting-started">Getting Started</a> &bull;<a href="#contributing">Contributing</a> &bull; <a href="#license">License</a> &bull; <a href="https://discord.gg/KQrEsbJ7cH">Discord</a></p>
<h2>Features</h2>
<p>The Dumpspace Generator and website supports</p>
<ul>
<li>Functions</li>
<li>Structs</li>
<li>Classes</li>
<li>Enums</li>
<li>Offsets</li>
</ul>
<p>Once implemented correctly, you can view your game on the website and search for what you need with a fluent design.</p>
<h2>Getting Started</h2>
<p>All you have to do to implement this library in your dumper is to copy and paste the&nbsp;<strong>DSGen.cpp</strong> and&nbsp;<strong>DSGen.h</strong> files into your project. C++20 is the standard of this project. The <strong>example.cpp</strong> file shows examples for all different types and edge cases for your functions, classes, structs or enums. You can always test the files by downloading the Dumpspace project itself and convert the output JSON files to .gzip files (use websites such as <a href="https://gzip.swimburger.net/">https://gzip.swimburger.net/</a>) and follow the project rules.</p>
<h2>Contributing</h2>
<p>Contribution is always appreciated! However, if any of the layout changes, the parsing on the website has to change too, and I doubt that I will change the parsing layout of the files in the future at any time.&nbsp;</p>
<h2>License</h2>
<p>This project is licensed under the terms of the MIT license.</p>
<h2>Showcase</h2>
<p>The dumper <a href="https://github.com/Spuckwaffel/UEDumper">UEDumper</a> has a "wish" variant of this library, but the output is the same, it just contains the old binaries.</p>
<p>Join the discord for any questions and help <a href="https://discord.gg/KQrEsbJ7cH">here.</a></p>
