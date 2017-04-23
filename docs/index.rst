.. cake-framework documentation master file, created by
   sphinx-quickstart on Fri Apr 21 12:33:45 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

CAKE Docs
==========================================

`Feedback <https://goo.gl/forms/GRWbj1zRdEylHJL32>`_

*Any application we see today, the especially big-data application will have goals/vision for ten years. Otherwise, it will only not work - a note from a Prof at Standford*


Cake
^^^^^^^^^^^^^^^^^^^^^
Cake Goals
------------------

- My Scheduling decision should take application level and system-level constraints.

    - Application Level Constraints.
        - example Jobs from the premium user should have the highest priority.

    - System Level constraints.
        - example, Machine or System XYZ is for ABC operations only.

- Example application
    -  Photo Storage App, where there are two types of users, premium/normal users. Now any job submitted by the user basically stores the photo in distributed file system(please note we have not told any specific file system, it could be anything). For premium users will have the advanced search feature but not for normal users. both users can search, in background the advance search happens in some powerful instances. Suppose there is no load



.. image :: /image/wip.jpg
=======
.. warning:: Work in progress, completed version will be releases on Jun 1,2017




.. toctree::
   :maxdepth: 2
   :numbered:
   :titlesonly:
   :glob:
   :hidden:

   Cake-code.rst
   developers.rst




Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
