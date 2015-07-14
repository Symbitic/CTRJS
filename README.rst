|Logo|

=======
 CTRJS
=======

|Release| |License| 

Native JavaScript interpreter for the Nintendo 3DS.

Introduction
------------

`CTRJS`_ is an abbreviation for *CTR JavaScript*, and offers a full
implementation of JavaScript for the Nintendo 3DS, with various extensions and
features to help bring JavaScript fully into the world of Nintendo 3DS homebrew
software development.

CTRJS aims to bring a full port of JavaScript to the Nintendo 3DS, using `V7`_
as the JavaScript engine, and `libctr`_ for providing platform-specific modules,
making it easier for developers to create homebrew, without needing to use
C/C++.

See https://symbitic.github.io/ctrjs/index.html for end-user downloads and
documentation.

Features
--------

* ECMAScript E5/E5.1 compliant.
* Nintendo 3DS specific modules, for easier porting and writing.
* Garbage collection.
* Asynchronous language and APIs reflect the nature inherent to modern video
  game programming and to ARM programming in general.
* Efficiency: Because of the languages' considerable popularity, JavaScript
  engines are highly-opimized. CTRJS gains even more performance because it
  sits closer to the hardware than do JavaScript engines running on a desktop.
* Portability: Excluding unfinished ports like the filesystem and networking,
  it should be relatively straightforward to port existing JavaScript to run on
  CTRJS.
* Quicker deployment, by offering a high-level language which can be written
  on the host (PC) or target (Nintendo 3DS).
* Modularity: Unlike most scripting languages, JavaScript has good support for
  organizing code into modules. A module in JavaScript can be as simple as a
  file in the same directory (e.x. ``main.js`` includes ``print.js``), or it
  can be an externally developed library imported from a package manager (e.x.
  writing a Node.js app that uses ExpressJS installed using NPM).
* Following Semantic Versioning, CTRJS offers a promise of compatibility. By
  offering a means for apps and libraries to declare which version of CTRJS they
  were written to run on, any existing code can continue to run when new
  versions are released, as long as the required versions can be found.
* JavaScript is *the* programming language of the web, and not without reason.
  It's syntax is very extensible and expressive. By given programmers the power
  to write whatever they want, however they want, JavaScript has helped make
  programming fun for a large number of people. Adding even greater value to
  this benefit is the fact that Nintendo has chosen to make their console as
  closed-nature as possible, even as it results in a critical lack of developer
  resources. Offering a "platform of fun" on a platform which has only a very
  limited offering of fun does have its advantages.

Installing
----------

CTRJS is not currently stable, and should not be considered production-ready.
When the stable version is released, pre-compiled packages will be available for
downloading. Until then, CTRJS must be built from source. See :ref:`Building`
for more details.

Building
--------

Building CTRJS requires `OpenCTR`_, including `libctr`_. Like most OpenCTR
projects, CTRJS uses CMake as the build system. An example of building CTRJS
from source, assuming that OpenCTR is installed and added to ``$PATH``:

.. code-block:: bash

   git clone https://github.com/Symbitic/CTRJS
   cd CTRJS
   mkdir build
   cd build
   cmake ..
   make

If everything was successful, you should now have a file named ``CTRJS.3dsx``
inside the ``build/`` directory.

License
-------

**CTRJS** is licensed under the `GNU General Public License v3.0`_. 

See `LICENSE.txt`_ for terms.

.. |Logo| image:: https://openctr.github.io/_static/logo.svg
   :alt: OpenCTR Logo
   :width: 250px
   :target: https://openctr.github.io/

.. |Release| image:: https://img.shields.io/github/release/Symbitic/CTRJS.svg?style=flat-square&label=Release
   :alt: Latest Release
   :target: https://github.com/OpenCTR/libctr/releases/latest

.. |License| image:: https://img.shields.io/github/license/Symbitic/CTRJS.svg?style=flat-square&label=License
   :alt: GNU General Public License v3.0
   :target: http://choosealicense.com/licenses/gpl-3.0/

.. _CTRJS: https://github.com/Symbitic/CTRJS

.. _V7: https://github.com/cesanta/v7

.. _OpenCTR: https://github.com/OpenCTR/OpenCTR

.. _libctr: https://github.com/OpenCTR/libctr

.. _`GNU General Public License v3.0`: http://www.gnu.org/licenses/gpl.html

.. _`LICENSE.txt`: ./LICENSE.txt

