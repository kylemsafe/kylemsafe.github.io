// src/MyForm.js

import React, { useState } from 'react';

const MyForm = () => {
    const [formData, setFormData] = useState({
        name: '',
        email: '',
        message: ''
    });

    const handleChange = (event) => {
        const { name, value } = event.target;
        setFormData(prevState => ({
            ...prevState,
            [name]: value
        }));
    };

    const handleSubmit = (event) => {
        event.preventDefault();
        // Handle form submission, e.g., send data to an API or display it
        console.log(formData);
    };

    return (
        <form onSubmit={handleSubmit}>
            <div>
                <label>
                    Name:
                    <input type="text" name="name" value={formData.name} onChange={handleChange} />
                </label>
            </div>
            <div>
                <label>
                    Email:
                    <input type="email" name="email" value={formData.email} onChange={handleChange} />
                </label>
            </div>
            <div>
                <label>
                    Message:
                    <textarea name="message" value={formData.message} onChange={handleChange} />
                </label>
            </div>
            <div>
                <button type="submit">Submit</button>
            </div>
        </form>
    );
};

export default MyForm;
