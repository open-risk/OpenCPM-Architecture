Riskmodel Dispatcher:

It connects models and data across different modules and systems via web services. The basic idea is that the result of one calculation is used as input of another, forming a three of calculations, dynamically dispatched. Uses a single point of dispatching.

How it works?

Requests for results are made to the dispatching server – request are query style. First element is the output of the required calculation, the second is the entity on which it is applied. Beyond these two fixed elements, the filter is augmented  with a sequence of not predefined keyvalue pairs. For instance an interest curve is requested via http://local host/?Target=CURVE& Method=BASR&Currency=USD. The dispatching server will return an XML atom feed that matches this query.

Install

Tested on Linux Mint Qiana. Python3.3 and LIbreOffice > 4.3 is required. The current version supports LibreOffice calcsheets. Requests from one calcsheet are dispatched to other sheets where data or results from calculations are retrieved.

Examples

In the example folder a simple bond pricing example. http://localhost:9001/?Target=PV yields the same result as in the Bond_Price_Yield_Zero_Curve.ods.

Licence

GNU

The mock-up is inspired by OpenGamma and how requirements are handled there. https://opengamma-docs.atlassian.net/wiki/display/DOC210/OpenGamma+Analytics+Value+Requirements
