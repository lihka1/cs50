from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    
    # get shares,sums,cash available...actual can be done in one step...
    shares = db.execute("SELECT Symbol FROM buy WHERE userid==:id GROUP BY Symbol  ORDER BY SUM(Shares) desc" ,id=session["user_id"])
    sums =  db.execute("SELECT SUM(Shares) FROM buy WHERE userid==:id GROUP BY Symbol  ORDER BY SUM(Shares) desc" ,id=session["user_id"])
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    
    # maintain total cash
    total = cash[0]["cash"]
    
    # store all results in one final dic...no nedd but for interset  
    final = {}
    
    # store the shares in one list
    shareList = []
    for share in shares:
        shareList.append(share["Symbol"])
        
    # store the total no of ( grouped by ) shares in one list
    sumsList = []
    for share in sums:
        sumsList.append(share["SUM(Shares)"])
        
    # see the present values of shaes and add them to the total
    for share,sum_i in zip(shareList, sumsList):
        dic = lookup(share)
        total += dic["price"]*sum_i
        final[dic["symbol"]] = (dic["name"],sum_i,dic["price"],dic["price"]*sum_i)
    
    # the index template
    return render_template("index.html",tot=usd(total),cash=usd(cash[0]["cash"]),final=final)
    

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        # get shares from the input
        share = request.form.get("symbol")
        num = request.form.get("shares")
        
        # lookup the price and validity  of share
        dic = lookup(share)
        if not dic:
            return apology("invalid symbol")
            
        # see the num of shares and validity
        if not num:
            return apology("invalid shares")
        if int(num) < 0:
            return apology("invalid shares")
    # get the cash u have present
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
    
    # check sufficient cash
    if cash[0]["cash"] < int(num)*dic["price"]:
        return apology("cannot afford")
    else:
        # update the database
        db.execute("INSERT INTO buy  VALUES (:Symbol,:Shares,:Price,datetime('now'),:id)", Symbol=share,\
                                                                       Shares=num, Price=dic["price"],id = session["user_id"]) 
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", id=session["user_id"], cash=cash[0]["cash"]-(int(num)*dic["price"]))
        
        # redirect user to home page
        flash("Bought!!")
        return redirect(url_for("index"))
            
@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    # get the history..recent first
    allrows = db.execute("SELECT * from buy WHERE userid=:id ORDER BY transacted desc",id=session["user_id"])
    
    return render_template("history.html", allrows=allrows)
    

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")
        
        
@app.route("/changePassword", methods=["GET", "POST"])
@login_required
def changePassword():
    if request.method == "GET":
        return render_template('changePswd.html')
    else:
        # get the passwords check for mismatch
        pass1 = request.form.get("newPaswd")
        pass2 = request.form.get("Re_newPaswd")
        if (pass1 != pass2):
            return apology("passwords dont match")
        else:
            # update the data-base
            db.execute("UPDATE users SET hash=:hash WHERE id==:id" , id=session["user_id"],hash=pwd_context.encrypt(pass1))
            return apology("password changed succesfully")
    
@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    if request.form.get("symbol"):
        dic = lookup(request.form.get("symbol"))
        if dic:
            return render_template("quotepost.html", dic=dic)
        else:
            return apology("invalid symbol")
    else:
         return apology("missing symbol")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "GET":
        # lets show the page
        return render_template("register.html")
        
    if request.method == "POST":
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")
        
            
        elif (not request.form.get("repassword")) or request.form.get("repassword") != request.form.get("password"):
            return apology("passwords dont match")
            
        else:
            # query database for username
            rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
            if len(rows) == 1:
                return apology("username taken")
            db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)" , username=request.form.get("username"), \
                                                                                        hash=pwd_context.encrypt(request.form.get("password")))
            # redirect user to login form
            # remember which user has logged in
            rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
            session["user_id"] = rows[0]["id"]

            # redirect user to home page
            return redirect(url_for("index"))

            
            

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    
    if request.method == "GET":
        return render_template("sell.html")
    else:
        share = request.form.get("symbol")
        num = request.form.get("shares")
        
        # check whether share is in bouht
        #rows = db.execute("SELECT DISTINCT Symbol FROM buy WHERE (userid=:id AND buy.Symbol == share AND buy.Shares > 0)", id=session["user_id"])
        rows = db.execute("SELECT DISTINCT Symbol FROM buy WHERE (userid==:id AND buy.Symbol == :share AND buy.Shares > 0)",id=session["user_id"],share=share)
        
        if not rows:
            return apology("shares not owned")
        
        # check num are valid
        if not num:
            return apology("invalid shares")
        
        # check whether that many shares can be sold
        rows = db.execute("SELECT DISTINCT Symbol FROM buy WHERE (userid==:id AND buy.Symbol == 'A' AND buy.Shares > 0)",id=session["user_id"])
        count = db.execute("SELECT  SUM(Shares) FROM buy WHERE userid==:id AND buy.Symbol == :share", id=session["user_id"],share=share)
        totcount = count[0]["SUM(Shares)"]

        # if can be sold ...then update the data base
        if totcount < int(num):
            return apology("too many shares than bought")
        else:
            # the cash got by selling the shares...currprice * int(num)
            dic = lookup(share)
            cashgot = int(num)*dic["price"]
            
            # the total cash
            cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
            totalcash = cashgot+cash[0]["cash"]
            
            # update changes....Shares,cash
            db.execute("INSERT INTO buy  VALUES (:Symbol,:Shares,:Price,datetime('now'),:id)", Symbol=dic["symbol"],\
                                                                        Shares="-"+num, Price=dic["price"],id = session["user_id"])
            
            db.execute("UPDATE users SET cash = :cash WHERE id = :id", id=session["user_id"], cash=totalcash)
            
            flash("Sold!!")
            return redirect(url_for("index"))
            

