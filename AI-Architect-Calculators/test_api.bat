@echo off
echo Testing AI Architect Calculators API
echo.

echo Test 1: ValueScore Calculator
curl -X POST http://localhost:8080/api/valueScore -H "Content-Type: application/json" -d "{\"scale\": 8, \"frequency\": 9, \"manualComplexity\": 6}"
echo.
echo.

echo Test 2: PERT Calculator
curl -X POST http://localhost:8080/api/pert -H "Content-Type: application/json" -d "{\"optimistic\": 5, \"mostLikely\": 10, \"pessimistic\": 20, \"unit\": \"days\"}"
echo.
echo.

echo Test 3: Risk Exposure Calculator
curl -X POST http://localhost:8080/api/risk -H "Content-Type: application/json" -d "{\"probabilityPercent\": 30, \"impact\": 50000, \"period\": \"/month\"}"
echo.
echo.

echo Test 4: TCO Calculator
curl -X POST http://localhost:8080/api/tco -H "Content-Type: application/json" -d "{\"capEx\": 100000, \"opExMonthly\": 5000, \"months\": 36}"
echo.
echo.

echo Test 5: Budget with Reserves Calculator
curl -X POST http://localhost:8080/api/budget -H "Content-Type: application/json" -d "{\"baseBudget\": 500000, \"knownReserve\": 50000, \"unknownPercent\": 20}"
echo.
echo.

echo All tests completed!
pause
