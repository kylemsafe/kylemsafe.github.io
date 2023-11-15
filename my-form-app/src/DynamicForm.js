import React, { useState } from 'react';
import formStructure from './formStructure.json'; // Adjust the path if needed

const DynamicForm = () => {
    const [formData, setFormData] = useState({});

    const handleChange = (event, parentPath) => {
        const { name, value } = event.target;
        const fullPath = parentPath ? `${parentPath}.${name}` : name;
        const nestedKeys = fullPath.split('.');
        const lastKey = nestedKeys.pop();
        const lastObj = nestedKeys.reduce((obj, key) => {
            if (key.endsWith(']')) {
                // Handling array index
                const match = key.match(/(.+)\[(\d+)\]/);
                return (match && obj[match[1]] && obj[match[1]][match[2]]) || {};
            }
            return (obj[key] = obj[key] || {});
        }, formData);
        lastObj[lastKey] = value;
        setFormData({ ...formData });
    };

    const renderFormFields = (data, parentPath = '') => {
        return Object.entries(data).map(([key, value]) => {
            const path = parentPath ? `${parentPath}.${key}` : key;

            if (value === null) {
                return (
                    <div key={path}>
                        <label>{key.replace(/\.\d+/g, '')}:</label>
                        <input 
                            type="text" 
                            name={key} 
                            value={formData[path] || ''} 
                            onChange={(e) => handleChange(e, parentPath)} 
                        />
                    </div>
                );
            } else if (Array.isArray(value)) {
                return (
                    <div key={path}>
                        <label>{key}:</label>
                        {value.map((item, index) => {
                            const itemPath = `${path}[${index}]`;
                            if (typeof item === 'object') {
                                return (
                                    <div key={itemPath}>
                                        {renderFormFields(item, itemPath)}
                                    </div>
                                );
                            } else {
                                return (
                                    <div key={itemPath}>
                                        <label>{item}:</label>
                                        <input 
                                            type="text" 
                                            name={itemPath} 
                                            value={formData[itemPath] || ''} 
                                            onChange={(e) => handleChange(e, itemPath)} 
                                        />
                                    </div>
                                );
                            }
                        })}
                    </div>
                );
            } else if (typeof value === 'object') {
                return (
                    <fieldset key={path}>
                        <legend>{key}</legend>
                        {renderFormFields(value, path)}
                    </fieldset>
                );
            }
            return null;
        });
    };

    const handleSubmit = (event) => {
        event.preventDefault();
        console.log(formData); // Handle form data as needed
    };

    return (
        <form onSubmit={handleSubmit}>
            {renderFormFields(formStructure)}
            <button type="submit">Submit</button>
        </form>
    );
};

export default DynamicForm;
