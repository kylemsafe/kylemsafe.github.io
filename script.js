document.addEventListener('DOMContentLoaded', function() {
    // Optionally, you can call createForm() here if you want the form to be pre-loaded
});

function login() {
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;

    // Basic validation - Replace with your own logic
    if (username === "admin" && password === "password") {
        document.getElementById('login-section').style.display = 'none';
        document.getElementById('main-section').style.display = 'block';
        createForm();
    } else {
        alert("Incorrect username or password!");
    }
}

function createForm() {
    const formSection = document.querySelector('.form-section');
    const form = document.createElement('form');
    form.innerHTML = `
        <!-- Your form fields here -->
        <!-- Example: -->
        <label for="name">Name:</label>
        <input type="text" id="name" name="name"><br><br>
        <!-- Add more form fields as needed -->
        <input type="button" value="Submit" onclick="submitForm()">
    `;

    formSection.appendChild(form);
}

function submitForm() {
    // Form submission logic
    // For demonstration, let's log the form data to the console
    console.log('Form submitted');
    // Add your logic to handle form submission
}

// Add additional JavaScript as needed
