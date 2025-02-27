`**Undetected webscraper build:**


Webscraper written in C flavoured C++, all libraries inlcuded within code. 
Most webscrapers assume you need javascript engine. However in 90% of webscraping applications that is not the case which results in significant increase in speed and decrease in resources required. 

Most webscrapers assume you need javascript engine to read dynamic content which is true. 
However in most webscraping projects/cases you only require data from the site. This is either static/provided by webserver and a simple curl command is sufficient.
Or the content is loaded dynamicaly by javascript. Rather than intrept the JS why not just copy what the JS is doing (more often that not simply querying an API). 

This is where undetected webscraper comes in. 
It has three JS-less functions. 

1. Static Webscraper (essentially just curl with lexbor HTML parsing and selector engine)
2. Static XML scaper similar to above but uses pugixml, slightly faster in cases where data is strict XML format. (sitemaps)
3. API scraper, this uses rapidjson and a custom recusive selector method to extract the data.


JS functions:
The webscraper will eventualy support JS operations via cPython running playwrite to extract data from websites that force JS to access or intial webserver provides extremely broken HTML so malformed that lexbor cannot pass it. 
This ofcourse will be slower due to cPython overhead (although this will be minimised after initial launch by maintaining browser window open). 

Todo:
1. Settings are currently stored in JSON file (loaded into memory on boot). Need front end design for building this json file. Manauly editing has lead to too many errors
2. Webserver/websockets integration. Front end template will be included but plan is to allow for custom front end running on external server with websockets coms to pass logs/commands and updated configs to main webscraper server.
3. Scheduler needs to be written
4. Significant levels of debugging


