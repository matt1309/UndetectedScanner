from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager

# Persistent Selenium WebDriver instance
driver = None


#can maybe add some more features like functions to navigate through website in the event that, 
#js actions are needed to bypass certain elements. 

#may be worth swapping to undetected_chromedriver that library might be dead, doesnt seem to 
#have had any updates in awhile so maybe another solution. 
def start_driver():
    global driver
    if driver is None:
        options = webdriver.ChromeOptions()
        options.add_argument("--headless")  # Run in headless mode (optional)
        driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()), options=options)

def seleniumScraper(retry, url, method, headers, useragent, proxy, timeout, request_data):
    """Loads a URL and returns page source"""
    global driver
    try:
        start_driver()  # Ensure driver is running
        driver.get(url)
        return {"status": "Ok", "data": driver.page_source}
    except Exception as e:
        driver.quit()
        driver = None  # Force restart next time
        return {"status": "Error", "data": str(e)}

def quit_driver():
    """Quits the WebDriver"""
    global driver
    if driver:
        driver.quit()
        driver = None
    return {"status": "Ok", "data": "Driver closed"}
