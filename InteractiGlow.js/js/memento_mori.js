// File: js/memento_mori.js (updated with country and flag logic)

if (!window.MementoMoriPage) {
    window.MementoMoriPage = {
        init: function() {
            // Populate the country dropdown as the first step
            this.populateCountries();

            const statusEl = document.getElementById('status');
            
            const updateConnectionUI = (statusMessage, isConnected) => {
                statusEl.textContent = statusMessage;
                statusEl.style.color = isConnected ? 'green' : 'red';
            };

            window.WebSocketService.addListener(updateConnectionUI);
            document.getElementById('calculateBtn').addEventListener('click', () => this.calculateAndSend());
            
            const initialState = window.WebSocketService.getConnectionStatus();
            updateConnectionUI(initialState.status, initialState.isConnected);
        },
        
        /**
         * Fetches the list of all countries from the World Bank API and populates the dropdown.
         */
        populateCountries: async function() {
            const countryDropdown = document.getElementById('country');
            const savedCountry = sessionStorage.getItem('mementoMoriCountry') || 'PT'; // Default to Portugal

            try {
                // The API is paginated, so we request a large number to get all countries in one go.
                const response = await fetch('https://api.worldbank.org/v2/country?format=json&per_page=300');
                const data = await response.json();

                if (!data || data.length < 2) {
                    throw new Error("Invalid API response for country list.");
                }

                const countries = data[1]; // The actual country data is in the second element
                countryDropdown.innerHTML = ''; // Clear the "Loading..." option

                countries.forEach(country => {
                    // We only want actual countries, not aggregates like "Euro area"
                    if (country.capitalCity) { 
                        const option = document.createElement('option');
                        option.value = country.iso2Code;
                        
                        // Convert the country code to a flag emoji
                        const flag = this.countryCodeToEmoji(country.iso2Code);
                        option.textContent = `${flag} ${country.name}`;
                        
                        countryDropdown.appendChild(option);
                    }
                });

                // Set the selected country to the one we saved
                countryDropdown.value = savedCountry;

            } catch (error) {
                console.error("Failed to load country list:", error);
                countryDropdown.innerHTML = '<option value="">Could not load countries</option>';
            }
        },

        /**
         * Converts a two-letter country code (like 'US') into a flag emoji (🇺🇸).
         * @param {string} isoCode - The two-letter ISO 3166-1 alpha-2 code.
         * @returns {string} The flag emoji.
         */
        countryCodeToEmoji: function(isoCode) {
            if (!isoCode || isoCode.length !== 2) {
                return '🏳️'; // Return a white flag for invalid codes
            }
            // Each letter is converted to a special Unicode character.
            // 'A' corresponds to Unicode 1F1E6, 'B' to 1F1E7, and so on.
            const codePoints = isoCode
                .toUpperCase()
                .split('')
                .map(char => 0x1F1E6 + (char.charCodeAt(0) - 'A'.charCodeAt(0)));
            return String.fromCodePoint(...codePoints);
        },

        calculateAndSend: async function() {
            const age = document.getElementById('age').value;
            const sex = document.getElementById('sex').value;
            const country = document.getElementById('country').value;
            const resultDiv = document.getElementById('result');

            // Save the selected country for next time
            sessionStorage.setItem('mementoMoriCountry', country);

            if (!age || !sex || !country) {
                alert('Please fill in all fields!');
                return;
            }

            resultDiv.textContent = 'Calculating...';

            try {
                const indicator = sex === 'M' ? 'SP.DYN.LE00.MA.IN' : 'SP.DYN.LE00.FE.IN';
                const response = await fetch(`https://api.worldbank.org/v2/country/${country}/indicator/${indicator}?format=json&date=2023`);
                const data = await response.json();
                
                if (!data[1] || !data[1][0] || !data[1][0].value) {
                    throw new Error('Life expectancy data not found for the selected country/year.');
                }
                
                const lifeExpectancy = parseFloat(data[1][0].value);
                
                const pastDays = Math.round(age * 365.25);
                const lifePercentage = ((age / lifeExpectancy) * 100).toFixed(2);
                const futureDays = Math.round((lifeExpectancy - age) * 365.25);

                resultDiv.innerHTML = `
                    Days Lived: ${pastDays}<br>
                    Life Percentage: ${lifePercentage}%<br>
                    Days Remaining: ${futureDays}
                `;

                const command = `LIFE:${pastDays}:${lifePercentage}:${futureDays}`;
                if (window.WebSocketService.send(command)) {
                    // Success message is optional
                } else {
                    alert('Failed to send data. Check the connection on the "Remote Control" page.');
                }
            } catch (error) {
                resultDiv.textContent = 'Error fetching life expectancy data.';
                console.error(error);
            }
        }
    };
}

window.MementoMoriPage.init();