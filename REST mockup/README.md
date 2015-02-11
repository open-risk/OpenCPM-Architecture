Riskmodel Dispatcher:

It connects models and data across different modules and systems via web services. The basic idea is that the result of one calculation is used as input of another, forming a tree of calculations, dynamically dispatched. Uses a single point of dispatching.

How it works?

Requests for results are made to the dispatching server – request are query style. First element is the output of the required calculation, the second is the entity on which it is applied. Beyond these two fixed elements, the filter is augmented  with a sequence of not predefined keyvalue pairs. For instance an interest curve is requested via http://local host/?Target=CURVE& Method=BASR&Currency=USD. The dispatching server will return an XML atom feed that matches this query.

Install

run python3.3 dispatch\ server.py in terminal. Stop the servers Ctrl+C (twice)
Tested on Linux Mint Qiana. Python3.3 and LibreOffice > 4.3 is required.

Examples

After starting the server, open the the simple bond pricing example Bond_Price_Yield_Zero_Curve.ods in the example folder. Targetting  http://localhost:9001/?Target=PV yields the same result as in the calcsheet.

Licence

GNU

The mock-up is inspired by OpenGamma and how requirements are handled there. https://opengamma-docs.atlassian.net/wiki/display/DOC210/OpenGamma+Analytics+Value+Requirements
