# ficklefridge

Fickle Fridge allows you to explore the statistic links between ingredients in recipes.
What ingredients go together? What can I make with what I have? What can I add to this recipe?

The application is written in PHP and Javascript with a custom static C++ database. It used to
use MySQL as the database, but in the end MySQL didn't provide the necessary performance. The
algorithm requires that every query visit every recipe in the database. In hindsight, a
parallel database like Hadoop would have been a better choice.

