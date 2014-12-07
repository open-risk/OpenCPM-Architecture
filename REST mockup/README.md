A RESTful approach:

1. A server with pyoo that accesses calc workbooks in libreoffice - here python 3 and libreoffice 4.3 are required. Problems with opening multiple instances of LibreOffice Calc.

2. REST server with OData support, pyslet seems to be the option - has to run python 2.7. A tutorial is here http://qtimigration.googlecode.com/svn/ ... ample.html. In-memory will do for now. The example data does not load properly.

3. A very simple OData model for the REST server as defined in the tutorial above: requirement (CURVE, PRESENT_VALUE, MARKET _VALUE and so on ), location( adress to the server in 1 where the calcbook resides). Then for each rule there will be a additional list of name/value pairs - currency=USF, Method=BASR.

4. Calc workbooks in which a range is defined corresponding to a requirement as defined in 3. A calc workbook will typically use ranges from other calc workbooks*. This is where the REST server comes in - The input to a calc workbook is done via filtered requests to the REST server; for instance a request for a CURVE with currency=USD, method=BASR.

*The use of calc workbooks is for illustration only, of course real databases, matlab, R and Python can be used as (end)nodes as well as the OData database itself.

This is a sketch and it could very well be faulty or useless.

TODO:

Write an R server to illustrate the concept as the LibreOffice server is not working.
