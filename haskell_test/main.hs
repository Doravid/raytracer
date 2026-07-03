import Control.Exception (assert)
import GHC.Float (sqrtFloat)

doubleMe :: (Num a) => a -> a
doubleMe x = x + x

tuple :: Float -> Float -> Float -> Float -> (Float, Float, Float, Float)
tuple x y z w = (x, y, z, w)

point :: Float -> Float -> Float -> (Float, Float, Float, Float)
point x y z = (x, y, z, 1.0)

vector :: Float -> Float -> Float -> (Float, Float, Float, Float)
vector x y z = (x, y, z, 0.0)

color :: Float -> Float -> Float -> (Float, Float, Float)
color r g b = (r, g, b)

r :: (Float, Float, Float) -> Float
r (r, g, b) = r

g :: (Float, Float, Float) -> Float
g (r, g, b) = g

b :: (Float, Float, Float) -> Float
b (r, g, b) = b

x :: (Float, Float, Float, Float) -> Float
x (x, y, z, w) = x

y :: (Float, Float, Float, Float) -> Float
y (x, y, z, w) = y

z :: (Float, Float, Float, Float) -> Float
z (x, y, z, w) = z

w :: (Float, Float, Float, Float) -> Float
w (x, y, z, w) = w

(===) :: Float -> Float -> Bool
(===) x y = abs (x - y) < 0.00001

(====) :: (Float, Float, Float, Float) -> (Float, Float, Float, Float) -> Bool
(====) (x, y, z, w) (a, b, c, d) = x === a && y === b && z === c && w === d

(|+) :: (Float, Float, Float, Float) -> (Float, Float, Float, Float) -> (Float, Float, Float, Float)
(|+) (x, y, z, w) (a, b, c, d) = (x + a, y + b, z + c, w + d)

(|-) :: (Float, Float, Float, Float) -> (Float, Float, Float, Float) -> (Float, Float, Float, Float)
(|-) (x, y, z, w) (a, b, c, d) = (x - a, y - b, z - c, w - d)

tNeg :: (Float, Float, Float, Float) -> (Float, Float, Float, Float)
tNeg (x, y, z, w) = (-x, -y, -z, -w)

tMult :: Float -> (Float, Float, Float, Float) -> (Float, Float, Float, Float)
tMult n (x, y, z, w) = (n * x, n * y, n * z, n * w)

tDiv :: Float -> (Float, Float, Float, Float) -> (Float, Float, Float, Float)
tDiv n (x, y, z, w) = (x / n, y / n, z / n, w / n)

vMagnitude :: (Float, Float, Float, Float) -> Float
vMagnitude (x, y, z, w) = sqrtFloat (x * x + y * y + z * z)

vNormalize :: (Float, Float, Float, Float) -> (Float, Float, Float, Float)
vNormalize (x, y, z, w) = tDiv (vMagnitude (x, y, z, w)) (x, y, z, w)

vDot :: (Float, Float, Float, Float) -> (Float, Float, Float, Float) -> Float
vDot (x, y, z, w) (a, b, c, d) = x * a + y * b + z * c + w * d

vCross :: (Float, Float, Float, Float) -> (Float, Float, Float, Float) -> (Float, Float, Float, Float)
vCross (x, y, z, w) (a, b, c, d) = (y * c - z * b, z * a - x * c, x * b - y * a, w)

colorAdd :: (Float, Float, Float) -> (Float, Float, Float) -> (Float, Float, Float)
colorAdd (r, g, b) (r2, g2, b2) = (r + r2, g + g2, b + b2)

colorMult :: Float -> (Float, Float, Float) -> (Float, Float, Float)
colorMult n (r, g, b) = (r * n, g * n, b * n)

canvas :: Int -> Int -> [[(Float, Float, Float)]]
canvas x y = replicate y (replicate x (0, 0, 0))

main :: IO ()
main = do
  assert (x (tuple 4.3 (-4.2) 3.1 1.0) == 4.3) (pure ())
  assert (y (tuple 4.3 (-4.2) 3.1 1.0) == -4.2) (pure ())
  assert (z (tuple 4.3 (-4.2) 3.1 1.0) == 3.1) (pure ())
  assert (w (tuple 4.3 (-4.2) 3.1 1.0) == 1.0) (pure ())

  --   Vector and Point Inits
  assert (vector 4 (-4) 3 ==== tuple 4.0 (-4.0) 3 0) (pure ())
  assert (point 4 (-4) 3 ==== tuple 4.0 (-4.0) 3 1.0) (pure ())
  -- Tuple Addition
  assert (tuple 3 (-2) 5 1 |+ tuple (-2) 3 1 0 ==== tuple 1 1 6 1.0) (pure ())
  -- Tuple Subtraction
  assert (vector 3 2 1 |- vector 5 6 7 ==== vector (-2) (-4) (-6)) (pure ())
  -- Tuple Negation
  assert (tNeg (vector 3 2 1) ==== vector (-3) (-2) (-1)) (pure ())
  -- Tuple Scaling
  assert (tMult 3.5 (tuple 1 (-2) 3 (-4)) ==== tuple 3.5 (-7) 10.5 (-14)) (pure ())
  -- Tuple Division
  assert (tDiv 2.0 (tuple 1 (-2) 3 (-4)) ==== tuple 0.5 (-1) 1.5 (-2)) (pure ())
  -- Vector Magnitude
  assert (vMagnitude (vector 1 0 0) === 1) (pure ())
  assert (vMagnitude (vector 1 2 3) === sqrtFloat 14) (pure ())
  assert (vMagnitude (vector (-1) (-2) (-3)) == sqrtFloat 14) (pure ())
  -- Vector Normalize
  assert (vNormalize (vector 4 0 0) ==== vector 1.0 0.0 0.0) (pure ())
  assert (vNormalize (vector 1 2 3) ==== vector (1.0 / sqrtFloat 14) (2.0 / sqrtFloat 14) (3.0 / sqrtFloat 14)) (pure ())
  assert (vMagnitude (vNormalize (vector 1 2 3)) === 1.0) (pure ())
  -- Vector Dot product
  assert (vDot (vector 1 2 3) (vector 2 3 4) === 20.0) (pure ())
  -- Vector Cross product
  assert (vCross (vector 1 2 3) (vector 2 3 4) ==== vector (-1) 2 (-1)) (pure ())
  assert (vCross (vector 2 3 4) (vector 1 2 3) ==== vector 1 (-2) 1) (pure ())

  putStrLn "Success"
